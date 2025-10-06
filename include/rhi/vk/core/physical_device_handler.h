//
// Created by ralex on 10/5/2025.
//

#ifndef RHI_PHYSICAL_DEVICE_HANDLER_H
#define RHI_PHYSICAL_DEVICE_HANDLER_H

#include "vk/core/extension_handler.h"
#include "vk/vulkan.h"

#include <vector>

namespace rhi::vk
{
    class physical_device
    {
    public:
        static physical_device create(VkPhysicalDevice, VkPhysicalDeviceProperties, VkPhysicalDeviceFeatures) noexcept;

    private:
        VkPhysicalDevice _handle { VK_NULL_HANDLE };
        VkPhysicalDeviceProperties _properties {};
        VkPhysicalDeviceFeatures _features {};

        extension_handler _extension_handler {};
    };

    class physical_device_handler
    {
    public:
        [[nodiscard]] physical_device_handler(VkInstance instance) noexcept;

    private:
        VkInstance _instance { VK_NULL_HANDLE };
        uint32_t _device_count { 0 };
        std::vector<physical_device> _devices;
    };
} // namespace rhi::vk

#endif //RHI_PHYSICAL_DEVICE_HANDLER_H