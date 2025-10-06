#include "vk/instance_vk.h"

#include <iostream>
#include <vulkan/vulkan_core.h>

#include "core/log.h"
#include "vk/core/extension_handler.h"
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
            && validation_handler.is_supported(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        if (use_debug)
        {
            requested_extensions.emplace_back(true, VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
            requested_layers.emplace_back(false, "VK_LAYER_KHRONOS_validation");
            log::debug("Requesting vulkan validation layer");
        }

        for (const auto& ext : _info.extensions)
        {
            requested_extensions.emplace_back(true, ext);
            log::debug("Requested extension: {}", ext);
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
            return unexpected(error(error::code::INSTANCE_FAIL, error_string));
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

        const VkInstanceCreateInfo instance_info {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .pApplicationInfo = &app_info,
            .enabledLayerCount = static_cast<uint32_t>(requested_layers.size()),
            .ppEnabledLayerNames = found_validation_layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(requested_extensions.size()),
            .ppEnabledExtensionNames = found_extensions.data()
        };

        const VkResult create_result = vkCreateInstance(
            &instance_info,
            nullptr,
            &inst._detail.instance
        );

        if (_info.window.has_value())
        {
            inst.create_surface(_info.window.value());
        }

        if (!vk_check(create_result))
        {
            return unexpected(error(error::code::INSTANCE_FAIL, "Failed to create instance -> [vkCreateInstance"));
        }
        log::debug("Instance creation completed successfully.");

        // Handle the physical devices


        return ok(inst);
    }

    expected<VkSurfaceKHR, surface_create_error> instance::create_surface(const window_data& window) noexcept
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
            _detail.instance,
            &win32_surface_create_info,
            nullptr,
            &surface
        );

        if (!vk_check(result))
        {
            return unexpected(surface_create_error::CREATE_SURFACE_FAILED);
        }
#endif // PLATFORM DETECTION

        return ok(surface);
    }

} // namespace rhi::vk