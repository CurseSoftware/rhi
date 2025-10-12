#include "vk/core/queue_family_indices.h"

#include <vector>
#include <ranges>

#include "core/format.h"

namespace rhi::vk
{
    queue_family_indices::queue_family_indices(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept
    {
        uint32_t family_count { 0 };
        std::vector<VkQueueFamilyProperties> properties {};

        vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, nullptr);
        properties.resize(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, properties.data());

        uint32_t idx { 0 };
        for (const auto& prop : properties)
        {
            if (_graphics_family_index && _present_family_index)
            {
                break;
            }

            // Graphics/compute
            {
                if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT && prop.queueFlags & VK_QUEUE_COMPUTE_BIT)
                {
                    _graphics_family_index = idx;
                }
            }

            // Present [VK_NULL_HANDLE for surface is assumed to be headless
            if (surface != VK_NULL_HANDLE) {
                VkBool32 present_support = VK_FALSE;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, idx, surface, &present_support);
                if (present_support)
                {
                    _present_family_index = idx;
                }
            }

            idx++;
        }
    }

    std::string queue_family_indices::get_formatted() const noexcept
    {
        const std::string graphics_string = _graphics_family_index.has_value()
        ? format_str("{}", _graphics_family_index.value())
            : "None";

        const std::string present_string = _present_family_index.has_value()
        ? format_str("{}", _present_family_index.value())
            : "None";

        return format_str("Graphics Index: {}. Present Index: {}", graphics_string, present_string);
    }

}
