#include "vk/core/debug_messenger.h"

#include "core/log.h"
#include "vk/core/utils.h"

namespace rhi::vk
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void* user_data
    )
    {
        const std::string_view type_prefix = [type]()
        {
            switch (type)
            {
            case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
                return "<GENERAL>    ";
            case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
                return "<VALIDATION> ";
            case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
                return "<PERFORMANCE>";
            default:
                return "<UNKNOWN>    ";
            }
        }();

        switch (severity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            log::debug("Vulkan Validation Layer {}: {}", type_prefix, callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            log::warn("Vulkan Validation Layer {}: {}", type_prefix, callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            log::error("Vulkan Validation Layer {}: {}", type_prefix, callback_data->pMessage);
            break;
        default:
            log::info("Vulkan Validation Layer {}: {}", type_prefix, callback_data->pMessage);
            break;
        }

        return VK_FALSE;
    }

    VkResult vk_create_debug_messenger(const VkInstance& instance, const VkDebugUtilsMessengerCreateInfoEXT& create_info, VkDebugUtilsMessengerEXT& messenger, VkAllocationCallbacks *allocation_callbacks = nullptr) noexcept
    {

        auto create_func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

        if (create_func != nullptr)
        {
            return create_func(instance, &create_info, allocation_callbacks, &messenger);
        }

        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void vk_destroy_debug_messenger(const VkInstance& instance, VkDebugUtilsMessengerEXT& messenger, VkAllocationCallbacks* allocation_callbacks = nullptr) noexcept
    {
        auto destroy_func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

        if (destroy_func != nullptr)
        {
            destroy_func(instance, messenger, allocation_callbacks);
        }
    }

    VkDebugUtilsMessengerCreateInfoEXT debug_messenger::get_create_info() noexcept
    {
        return VkDebugUtilsMessengerCreateInfoEXT {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                // VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = debug_callback,
            .pUserData = nullptr,
        };
    }

    expected<debug_messenger, std::string> debug_messenger::create(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT& create_info) noexcept
    {
        debug_messenger messenger {};
        messenger._instance = instance;

        const VkResult result = vk_create_debug_messenger(instance, create_info, messenger._debug_messenger);
        if (!vk_check(result))
        {
            if (result == VK_ERROR_EXTENSION_NOT_PRESENT)
            {
                log::error("Debug extension not present!");
                return unexpected(std::string("Vulkan Debug extension not present."));
            }
            log::error("Failed to create debug messenger!");
            return unexpected<std::string>("Failed to create debug messenger.");
        }

        return ok(messenger);
    }

    void debug_messenger::destroy() noexcept
    {
        log::debug("Destroying debug messenger...");
        vk_destroy_debug_messenger(_instance, _debug_messenger, nullptr);
        log::debug("Debug messenger destroyed.");
    }

} // namespace rhi::vk