#include "vk/core/physical_device_handler.h"

#include "core/log.h"
#include "vk/core/utils.h"

namespace rhi::vk
{
    physical_device_handler::physical_device_handler(const VkInstance instance, const VkSurfaceKHR surface) noexcept
        : _instance{ instance }
        , _surface{ surface }
    {

        vkEnumeratePhysicalDevices(_instance, &_device_count, nullptr);
        std::vector<VkPhysicalDevice> physical_devices(_device_count);
        vkEnumeratePhysicalDevices(_instance, &_device_count, physical_devices.data());

        for (const VkPhysicalDevice physical_device : physical_devices)
        {
            VkPhysicalDeviceProperties device_props {};
            VkPhysicalDeviceFeatures device_features {};

            vkGetPhysicalDeviceProperties(physical_device, &device_props);
            vkGetPhysicalDeviceFeatures(physical_device, &device_features);

            auto device = physical_device::create(physical_device, device_props, device_features);

            _devices.push_back(device);
        }
    }

    void physical_device_handler::request_extension(const std::string& extension_name, bool required) noexcept
    {
        _extensions.emplace_back(required, extension_name);
    }

    bool physical_device_handler::_is_device_suitable(class physical_device& device) const noexcept
    {
        const auto device_queue_family_indices = device.get_queue_family_indices(_surface);

        //
        if (_surface == VK_NULL_HANDLE)
        {
            log::warn("Physical device handler has VK_NULL_HANDLE for surface. Assuming headless rendering!");
            if (!device_queue_family_indices.has_graphics())
            {
                log::debug("Device {} does not have valid graphics queue index.", device.name());
                return false;
            }
        }
        else
        {
            // If we are not headless rendering, we still require graphics but also we need the present index
            if (!device_queue_family_indices.is_complete())
            {
                log::debug("Device {} does not have valid queue family indices: {}", device.name(), device_queue_family_indices.get_formatted());
                return false;
            }

            auto swapchain_support_info_exp = device.get_swapchain_support(_surface);
            if (!swapchain_support_info_exp.has_value())
            {
                log::error("Error getting support for swapchain for device [{}]: {}", device.name(), swapchain_support_info_exp.unwrap_error());
                return false;
            }

            if (auto swapchain_support_info = swapchain_support_info_exp.unwrap();
                swapchain_support_info.formats.empty()
                || swapchain_support_info.present_modes.empty())
            {
                return false;
            }
        }


        if (!device.max_sampler_anisotropy())
        {
            log::debug("Device {} does not have valid max sampler anisotropy", device.name());
            return false;
        }

        for (const auto& [is_required, extension_name] : _extensions)
        {
            if (is_required && !device.is_extension_supported(extension_name.c_str()))
            {
                log::debug("Device {} does not support required extension: {}", device.name(), extension_name);
                return false;
            }
        }

        return true;
    }

    void physical_device_handler::request_extensions(const std::vector<requested_extension>& extensions) noexcept
    {
        for (const auto& extension : extensions)
        {
            _extensions.emplace_back(extension);
        }
    }

    std::vector<physical_device> physical_device_handler::get_suitable_devices() const noexcept
    {
        log::debug("Getting suitable vulkan physical devices...");
        std::vector<physical_device> suitable_devices {};
        for (auto device : _devices)
        {
            log::debug("Checking {}...", device.name());
            if (_is_device_suitable(device))
            {
                log::debug("Suitable.");
                suitable_devices.emplace_back(device);
            }
            else
            {
                log::debug("Not suitable.");
            }
        }

        return suitable_devices;
    }


    void physical_device::_get_extensions() noexcept
    {
        uint32_t extension_count { 0 };
        vkEnumerateDeviceExtensionProperties(_handle, nullptr, &extension_count, nullptr);
        _extensions.resize(extension_count);
        vkEnumerateDeviceExtensionProperties(_handle, nullptr, &extension_count, _extensions.data());
    }

    VkSampleCountFlags physical_device::_get_max_sample_count() const noexcept
    {
        const VkSampleCountFlags sample_counts =
            _properties.limits.framebufferColorSampleCounts & _properties.limits.framebufferDepthSampleCounts;

        if (sample_counts & VK_SAMPLE_COUNT_64_BIT) return VK_SAMPLE_COUNT_64_BIT;
        if (sample_counts & VK_SAMPLE_COUNT_32_BIT) return VK_SAMPLE_COUNT_32_BIT;
        if (sample_counts & VK_SAMPLE_COUNT_16_BIT) return VK_SAMPLE_COUNT_16_BIT;
        if (sample_counts & VK_SAMPLE_COUNT_8_BIT) return VK_SAMPLE_COUNT_8_BIT;
        if (sample_counts & VK_SAMPLE_COUNT_4_BIT) return VK_SAMPLE_COUNT_4_BIT;
        if (sample_counts & VK_SAMPLE_COUNT_2_BIT) return VK_SAMPLE_COUNT_2_BIT;
        return VK_SAMPLE_COUNT_1_BIT;
    }



    physical_device physical_device::create(VkPhysicalDevice p_device, VkPhysicalDeviceProperties p_properties, VkPhysicalDeviceFeatures p_features) noexcept
    {
        log::debug("Creating vulkan physical device: {}", p_properties.deviceName);
        physical_device device {};
        device._handle = p_device;
        device._properties = p_properties;
        device._features = p_features;

        log::debug("Properties: \n{}", format_physical_device_properties(device._properties, "\t"));

        device._get_extensions();

        device._max_sample_count = device._get_max_sample_count();
        device._max_sampler_anisotropy = p_properties.limits.maxSamplerAnisotropy;

        return device;
    }

    bool physical_device::is_extension_supported(const char* extension_name) const noexcept
    {
        for (const auto& extension : _extensions)
        {
            if (strcmp(extension_name, extension.extensionName) == 0)
            {
                return true;
            }
        }

        return false;
    }

    expected<swapchain_support, std::string> physical_device::get_swapchain_support(const VkSurfaceKHR surface) noexcept
    {
        if (_swapchain_support.has_value())
        {
            return ok(_swapchain_support.value());
        }

        swapchain_support support {};
        uint32_t format_count { 0 }
                , present_count { 0 }
        ;
        VkSurfaceFormatKHR *dummy_formats { nullptr };

        vkGetPhysicalDeviceSurfaceFormatsKHR(_handle, surface, &format_count, dummy_formats);
        support.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(_handle, surface, &format_count, support.formats.data());

        vkGetPhysicalDeviceSurfacePresentModesKHR(_handle, surface, &present_count, nullptr);
        support.present_modes.resize(present_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(_handle, surface, &present_count, support.present_modes.data());

        if (const VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_handle, surface, &support.capabilities);!vk_check(result))
        {
            return unexpected<std::string>("Failed to retrieve device surface capabilities");
        }

        _swapchain_support = support;

        return ok(support);
    }



    queue_family_indices physical_device::get_queue_family_indices(const VkSurfaceKHR& surface) const noexcept
    {
        return queue_family_indices{ _handle, surface };
    }


}
