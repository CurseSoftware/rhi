#include "vk/instance_vk.h"

#include <iostream>
#include <vulkan/vulkan_core.h>

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
        }

        for (const auto& ext : _info.extensions)
        {
            requested_extensions.emplace_back(true, ext);
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

        auto validation_layers_exp = validation_handler.get_requested(requested_layers);
        auto extensions_exp = instance_extension_handler.get_requested(requested_extensions);

        if (!validation_layers_exp.has_value())
        {
            auto error_string = validation_layers_exp.unwrap_error();
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

        const enum VkResult create_result = vkCreateInstance(
            &instance_info,
            nullptr,
            &inst._detail.instance
        );

        if (!vk_check(create_result))
        {
            return unexpected(error(error::code::INSTANCE_FAIL, "Failed to create instance -> [vkCreateInstance"));
        }

        return ok(inst);
    }

} // namespace rhi::vk