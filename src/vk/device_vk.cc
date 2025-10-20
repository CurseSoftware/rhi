#include "vk/device_vk.h"

#include "core/format.h"
#include "core/log.h"
#include "vk/core/utils.h"

namespace rhi::vk
{
    expected<device, std::string> device::builder::build() noexcept
    {
        class device device {};
        const auto& indices = _info.physical_device.get_queue_family_indices(_info.surface);

        const std::vector<VkDeviceQueueCreateInfo> queue_create_infos = indices.get_create_infos();
        const auto features = _info.physical_device.get_features();

        log::debug("Creating device...");
        log::debug("Available extensions: ");
        for (const auto ext : _info.extensions)
        {
            log::debug("\t{}", ext);
        }

        const VkDeviceCreateInfo create_info {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
            .pQueueCreateInfos = queue_create_infos.data(),
            .enabledExtensionCount = static_cast<uint32_t>(_info.extensions.size()),
            .ppEnabledExtensionNames = _info.extensions.data(),
            .pEnabledFeatures = std::addressof(features)
        };

        const VkResult create_result = vkCreateDevice(_info.physical_device.get(), &create_info, nullptr, &device._device);

        if (!vk_check(create_result))
        {
            const auto message = format_str("Failed to create device. vkCreateDevice failed with {}", vulkan_result_to_string(create_result));
            return unexpected(message);
        }

        return ok(device);
    }

    device::builder& device::builder::surface(const VkSurfaceKHR& surface) noexcept
    {
        _info.surface = surface;

        return *this;
    }

    device::builder& device::builder::request_extension(const char* extension) noexcept
    {
        _info.extensions.push_back(extension);

        return *this;
    }

    device::builder& device::builder::request_extensions(const std::span<const char*> extensions) noexcept
    {
        auto& existing_extensions = _info.extensions;
        auto already_requested = [&existing_extensions](const char* new_extension) noexcept
        {
            for (const char* ext : existing_extensions)
            {
                if (strcmp(new_extension, ext) == 0)
                {
                    return true;
                }
            }

            return false;
        };

        for (auto& extension: extensions)
        {
            if (!already_requested(extension))
            {
                existing_extensions.push_back(extension);
            }
        }

        return *this;
    }

    void device::destroy() noexcept
    {
        vkDestroyDevice(_device, nullptr);
    }

} // namespace rhi::vk