//
// Created by ralex on 9/28/2025.
//

#ifndef RHI_DEVICE_VK_H
#define RHI_DEVICE_VK_H
#ifdef BACKEND_USE_VULKAN

#include "vk/vulkan.h"

namespace rhi::vk
{
    struct device_context
    {
        VkPhysicalDevice physical_device { VK_NULL_HANDLE };
    };
} // namespace rhi::vk

#endif // BACKEND_USE_VULKAN

#endif //RHI_DEVICE_VK_H