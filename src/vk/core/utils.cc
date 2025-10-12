//
// Created by ralex on 10/5/2025.
//

#include "vk/core/utils.h"
#include "vk/vulkan.h"

#include <exception>
#include <iostream>

#include "core/format.h"

namespace rhi::vk
{
    void vk_assert(const VkResult result, const char* msg) noexcept
    {
        if (result != VK_SUCCESS)
        {
            std::cerr << msg << std::endl;
            std::terminate();
        }
    }

    bool vk_check(VkResult result) noexcept
    {
        return result == VK_SUCCESS;
    }

    const char* format_physical_device_type(VkPhysicalDeviceType device_type) noexcept
    {
        switch (device_type)
        {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            return "OTHER";
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            return "INTEGRATED_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            return "CPU";
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            return "DISCRETE_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            return "VIRTUAL_GPU";

        default:
            return "UNKNOWN";
        }
    }


    std::string format_physical_device_properties(const VkPhysicalDeviceProperties& properties, const std::string& prefix) noexcept
    {
        std::string properties_string { "" };

        properties_string += format_str("{}Device Name:    {}\n", prefix, properties.deviceName);
        properties_string += format_str("{}API Version:    {}\n", prefix, properties.apiVersion);
        properties_string += format_str("{}Device ID:      {}\n", prefix, properties.deviceID);
        properties_string += format_str("{}Device Type:    {}\n", prefix, format_physical_device_type(properties.deviceType));
        properties_string += format_str("{}Vendor ID:      {}\n", prefix, properties.vendorID);
        properties_string += format_str("{}Driver Version: {}\n", prefix, properties.driverVersion);

        return properties_string;
    }


} // rhi::namespace