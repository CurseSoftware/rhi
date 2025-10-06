#include "vk/core/physical_device_handler.h"

namespace rhi::vk
{
    physical_device_handler::physical_device_handler(VkInstance instance) noexcept
        : _instance{ instance }
    {
        vkEnumeratePhysicalDevices(_instance, &_device_count, nullptr);
        std::vector<VkPhysicalDevice> physical_devices(_device_count);
        vkEnumeratePhysicalDevices(_instance, &_device_count, physical_devices.data());
    }

    physical_device physical_device::create(VkPhysicalDevice p_device, VkPhysicalDeviceProperties p_properties, VkPhysicalDeviceFeatures p_features) noexcept
    {
        physical_device device {};
        device._handle = p_device;
        device._properties = p_properties;
        device._features = p_features;

        return device;
    }

}