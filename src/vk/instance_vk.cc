#include "vk/instance_vk.h"

#include <iostream>
#include <memory>

#include "core/debug_messenger.h"
#include "core/log.h"
#include "vk/device_vk.h"
#include "vk/core/extension_handler.h"
#include "vk/core/physical_device_handler.h"
#include "vk/core/utils.h"
#include "vk/core/validation_handler.h"

namespace rhi::vk
{
    expected<instance, error> instance::builder::build() const noexcept
    {
        instance inst {};

        validation_layer_handler validation_handler {};
        std::vector<requested_layer> requested_layers {};

        extension_handler instance_extension_handler {};
        std::vector<requested_extension> requested_extensions {};

        instance_extension_handler.fetch_all_extensions<instance>();

        const bool use_debug
            = _info.enable_debug
            && instance_extension_handler.is_supported(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        if (use_debug)
        {
            log::debug("Use debug option is enabled.");
            requested_extensions.emplace_back(true, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            requested_layers.emplace_back(false, "VK_LAYER_KHRONOS_validation");
        }

        for (const auto& ext : _info.extensions)
        {
            requested_extensions.emplace_back(true, ext);
            log::debug("Requested extension: {}", ext);
        }

        if (_info.enable_debug)
        {
            requested_layers.emplace_back(false, "VK_LAYER_KHRONOS_validation");
        }

        if (!_info.headless)
        {
            requested_extensions.emplace_back(true, VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef RHI_PLATFORM_WINDOWS
            requested_extensions.emplace_back(true, VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(RHI_PLATFORM_LINUX)
            requested_extensions.emplace_back(true, VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif
        }
        // NOTE: not all drivers support this extension. NVIDIA & AMD do not.
        // else
        // {
        //     requested_extensions.emplace_back(true, VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME);
        // }

        const auto validation_layers_exp = validation_handler.get_requested(requested_layers);
        const auto extensions_exp = instance_extension_handler.get_requested(requested_extensions);

        if (!validation_layers_exp.has_value())
        {
            const auto error_string = validation_layers_exp.unwrap_error();
            log::error("Instance creation failed with message: [{}]", error_string);
            return unexpected(error(error::code::INSTANCE_FAIL, error_string));
        }

        if (!extensions_exp.has_value())
        {
            const auto error_string = extensions_exp.unwrap_error();
            log::error("Instance creation failed with message: [{}]", error_string);
            return unexpected(error(error::code::INSTANCE_FAIL, error_string.c_str()));
        }

        auto found_validation_layers = validation_layers_exp.unwrap();
        auto found_extensions = extensions_exp.unwrap();

        VkApplicationInfo app_info {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "RHI_VK_APPLICATION",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "NO_ENGINE",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_0
        };

        VkInstanceCreateInfo instance_info {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .pApplicationInfo = &app_info,
            .enabledLayerCount = static_cast<uint32_t>(requested_layers.size()),
            .ppEnabledLayerNames = found_validation_layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(requested_extensions.size()),
            .ppEnabledExtensionNames = found_extensions.data()
        };

        std::unique_ptr<VkDebugUtilsMessengerCreateInfoEXT> debug_create_info;
        if (_info.enable_debug)
        {
            debug_create_info = std::make_unique<VkDebugUtilsMessengerCreateInfoEXT>(debug_messenger::get_create_info());
            instance_info.pNext = debug_create_info.get();
        }

        const VkResult create_result = vkCreateInstance(
            &instance_info,
            nullptr,
            &inst._handle
        );

        if (_info.window.has_value())
        {
            auto surface_exp = inst.create_surface(_info.window.value());
            if (!surface_exp.has_value())
            {
                return unexpected(error(error::code::INSTANCE_FAIL, "Failed to create instance -> [instance::create_surface]"));
            }

            inst._surface = surface_exp.unwrap();
            log::debug("Surface created.");
        }

        if (!vk_check(create_result))
        {
            return unexpected(error(error::code::INSTANCE_FAIL, "Failed to create instance -> [vkCreateInstance"));
        }
        log::debug("Instance creation completed successfully.");


        if (_info.enable_debug)
        {
            log::debug("Creating debug messenger...");
            auto messenger_exp = debug_messenger::create(inst._handle, *debug_create_info.get());
            if (!messenger_exp.has_value())
            {
                log::error("Failed to create debug messenger: [{}]", messenger_exp.unwrap_error());
                return unexpected(error(error::code::INSTANCE_FAIL, "Failed to create debug messenger"));
            }
            auto messenger = messenger_exp.unwrap();
            inst._debug_messenger = std::make_unique<debug_messenger>(messenger);
            log::debug("Debug messenger created.");
        }

        // Handle the physical devices
        log::debug("Retrieving suitable physical devices...");
        physical_device_handler pd_handler { inst._handle, inst._surface };
        if (!_info.headless)
        {
            inst._device_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
            pd_handler.request_extension(VK_KHR_SURFACE_EXTENSION_NAME, true);
        }
        inst._suitable_devices = pd_handler.get_suitable_devices();

        if (inst._suitable_devices.empty())
        {
            return unexpected(error(error::code::INSTANCE_FAIL, "No suitable physical devices found"));
        }

        // return ok<instance>(std::move(inst));
        return ok(inst);
    }

    expected<class device, std::string> instance::create_device() noexcept
    {
        const auto pd = _suitable_devices[0];

        auto device_exp = device::builder(pd)
            .surface(_surface)
            .request_extensions(_device_extensions)
            .build();

        if (!device_exp.has_value())
        {
            return unexpected(device_exp.unwrap_error());
        }

        _managed_devices.push_back(device_exp.unwrap());

        return device_exp;
    }

    auto instance::create_device(uint32_t physical_device_id) noexcept -> expected<class device, std::string>
    {
        const auto device_attempt = [this, physical_device_id]() -> std::tuple<physical_device, bool>
        {
            constexpr bool device_found = true;
            constexpr bool device_not_found = false;

            for (auto& device : this->_suitable_devices)
            {
                if (device.id() == physical_device_id)
                {
                    return std::make_tuple(device, device_found);
                }
            }

            return std::make_tuple(_suitable_devices[0], device_not_found);
        }();

        if (!std::get<bool>(device_attempt))
        {
            std::string message = format_str("Device with id {} was not found in list of suitable devices.", physical_device_id);
            return unexpected(message);
        }

        const auto pd = std::get<physical_device>(device_attempt);

        auto device_exp = device::builder(pd)
            .surface(_surface)
            .request_extensions(_device_extensions)
            .build();

        if (!device_exp.has_value())
        {
            return unexpected(device_exp.unwrap_error());
        }

        _managed_devices.push_back(device_exp.unwrap());

        return device_exp;
    }



    expected<VkSurfaceKHR, surface_create_error> instance::create_surface(const window_data& window) const noexcept
    {
        VkSurfaceKHR surface { VK_NULL_HANDLE };

#if defined(RHI_PLATFORM_WINDOWS)
        if (!std::get_if<window_data_win32>(&window))
        {
            return unexpected(surface_create_error::INVALID_WINDOW_DATA_NOT_WIN32);
        }

        const auto& window_description = std::get<window_data_win32>(window);

        VkWin32SurfaceCreateInfoKHR win32_surface_create_info {
            .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .pNext = nullptr,
            .hwnd = window_description.hwnd
        };

        const VkResult result = vkCreateWin32SurfaceKHR(
            _handle,
            &win32_surface_create_info,
            nullptr,
            &surface
        );

        if (!vk_check(result))
        {
            return unexpected(surface_create_error::CREATE_SURFACE_FAILED);
        }
#endif // PLATFORM DETECTION
        // TODO: perform surface creation for other windowing platforms

        return ok(surface);
    }

    void instance::destroy() noexcept
    {
        log::debug("Destroying instance...");
        _debug_messenger->destroy();
        _debug_messenger.reset();

        for (auto& device : _managed_devices)
        {
            device.destroy();
        }

        _suitable_devices.clear();
        if (_surface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(_handle, _surface, nullptr);
        }

        vkDestroyInstance(_handle, nullptr);
        log::debug("Instance destroyed");
    }


} // namespace rhi::vk