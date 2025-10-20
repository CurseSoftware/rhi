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

    std::string vulkan_result_to_string(const VkResult result) noexcept
    {
#define RHI_VULKAN_RESULT_TO_STRING(name) case name: return #name;
        switch (result)
        {
            RHI_VULKAN_RESULT_TO_STRING(VK_SUCCESS);
            RHI_VULKAN_RESULT_TO_STRING(VK_NOT_READY);
            RHI_VULKAN_RESULT_TO_STRING(VK_TIMEOUT);
            RHI_VULKAN_RESULT_TO_STRING(VK_EVENT_SET);
            RHI_VULKAN_RESULT_TO_STRING(VK_EVENT_RESET);
            RHI_VULKAN_RESULT_TO_STRING(VK_INCOMPLETE);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_OUT_OF_HOST_MEMORY);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_OUT_OF_DEVICE_MEMORY);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_INITIALIZATION_FAILED);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_DEVICE_LOST);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_MEMORY_MAP_FAILED);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_LAYER_NOT_PRESENT);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_EXTENSION_NOT_PRESENT);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_FEATURE_NOT_PRESENT);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_INCOMPATIBLE_DRIVER);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_TOO_MANY_OBJECTS);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_FORMAT_NOT_SUPPORTED);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_FRAGMENTED_POOL);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_UNKNOWN);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_VALIDATION_FAILED_EXT);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_OUT_OF_POOL_MEMORY);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_INVALID_EXTERNAL_HANDLE);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_FRAGMENTATION);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS);
            RHI_VULKAN_RESULT_TO_STRING(VK_PIPELINE_COMPILE_REQUIRED);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_NOT_PERMITTED_EXT);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_SURFACE_LOST_KHR);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
            RHI_VULKAN_RESULT_TO_STRING(VK_SUBOPTIMAL_KHR);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_OUT_OF_DATE_KHR);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_INVALID_SHADER_NV);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
            RHI_VULKAN_RESULT_TO_STRING(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT);
            RHI_VULKAN_RESULT_TO_STRING(VK_THREAD_IDLE_KHR);
            RHI_VULKAN_RESULT_TO_STRING(VK_THREAD_DONE_KHR);
            RHI_VULKAN_RESULT_TO_STRING(VK_OPERATION_DEFERRED_KHR);
            RHI_VULKAN_RESULT_TO_STRING(VK_OPERATION_NOT_DEFERRED_KHR);
        default:
            return "VK_UNKNOWN_ERROR";
        }
#undef RHI_VULKAN_RESULT_TO_STRING
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