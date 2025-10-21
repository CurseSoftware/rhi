//
// Created by ralex on 10/5/2025.
//

#ifndef RHI_PHYSICAL_DEVICE_HANDLER_H
#define RHI_PHYSICAL_DEVICE_HANDLER_H

#include "vk/core/extension_handler.h"
#include "vk/vulkan.h"

#include <vector>

#include "vk/core/queue_family_indices.h"

namespace rhi::vk
{
    struct swapchain_support
    {
        VkSurfaceCapabilitiesKHR capabilities       {};
        std::vector<VkSurfaceFormatKHR> formats     {};
        std::vector<VkPresentModeKHR> present_modes {};
    };

    class physical_device final : public vulkan_object<VkPhysicalDevice>
    {
    public:
        [[nodiscard]] static physical_device create(VkPhysicalDevice, VkPhysicalDeviceProperties, VkPhysicalDeviceFeatures) noexcept;

        [[nodiscard]] queue_family_indices get_queue_family_indices(const VkSurfaceKHR& surface) const noexcept;

        [[nodiscard]] const std::vector<VkExtensionProperties>& get_device_extensions() const noexcept { return _extensions; };
        [[nodiscard]] VkSampleCountFlags max_sample_count() const noexcept { return _max_sample_count; }
        [[nodiscard]] float max_sampler_anisotropy() const noexcept { return _max_sampler_anisotropy; }
        [[nodiscard]] VkPhysicalDeviceFeatures get_features() const noexcept { return _features; }

        [[nodiscard]] bool is_extension_supported(const char*) const noexcept;
        [[nodiscard]] expected<swapchain_support, std::string> get_swapchain_support(const VkSurfaceKHR) noexcept;

        [[nodiscard]] std::string_view name() const noexcept { return _properties.deviceName; }

        [[nodiscard]] auto id() const noexcept -> uint32_t { return _properties.deviceID; }

    private:
        void _get_extensions() noexcept;
        [[nodiscard]] VkSampleCountFlags _get_max_sample_count() const noexcept;

    private:
        VkPhysicalDeviceProperties _properties {};
        VkPhysicalDeviceFeatures _features {};
        VkSampleCountFlags _max_sample_count {};
        float _max_sampler_anisotropy { 0.0f };
        std::optional<swapchain_support> _swapchain_support {};

        std::vector<VkExtensionProperties> _extensions {};

        extension_handler _extension_handler {};
    };

    class physical_device_handler
    {
    public:
        [[nodiscard]] explicit physical_device_handler(VkInstance instance, VkSurfaceKHR surface) noexcept;

        void request_extension(const std::string&, bool = true) noexcept;
        void request_extensions(const std::vector<requested_extension>&) noexcept;

        [[nodiscard]] std::vector<physical_device> get_suitable_devices() const noexcept;

    private:
        [[nodiscard]] bool _is_device_suitable(physical_device&) const noexcept;

    private:
        VkInstance _instance { VK_NULL_HANDLE };
        VkSurfaceKHR _surface { VK_NULL_HANDLE };
        uint32_t _device_count { 0 };
        std::vector<physical_device> _devices;

        std::vector<requested_extension> _extensions {};
    };
} // namespace rhi::vk

#endif //RHI_PHYSICAL_DEVICE_HANDLER_H