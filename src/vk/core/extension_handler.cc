#include "vk/core/extension_handler.h"
#include "core/log.h"
#include "vk/vulkan.h"
#include <cstdint>
#include <iostream>
#include <vector>

#include "vk/instance_vk.h"

namespace rhi::vk
{
    template <>
    void extension_handler::fetch_all_extensions<struct instance>() noexcept
    {
        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
        _extensions.resize(extension_count);
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, _extensions.data());
    }

    template <>
    void extension_handler::fetch_all_extensions<struct physical_device>() noexcept
    {
    }

    expected<std::vector<const char*>, const char*> extension_handler::get_requested(const std::span<requested_extension> requested_extensions) const noexcept
    {
        std::vector<const char*> found_extensions {};

        for (const auto& extension : requested_extensions)
        {
            if (!is_supported(extension.name))
            {
                if (extension.is_required)
                {
                    log::error("Vulkan extension {} is missing.", extension.name);
                    return unexpected(extension.name.c_str());
                }
                else
                {
                    log::warn("Non-required Vulkan extension {} is missing.", extension.name);
                }
            }
            else
            {
                log::debug("Vulkan extension {} found.", extension.name);
                found_extensions.push_back(extension.name.c_str());
            }
        }
        log::debug("All requested vulkan extensions found.");

        return ok(found_extensions);
    }



    bool extension_handler::is_supported(std::string_view extension_name) const noexcept
    {
        for (const auto& [extensionName, specVersion] : _extensions)
        {
            if (strcmp(extensionName, extension_name.data()) == 0)
            {
                return true;
            }
        }

        return false;
    }


} // namespace rhi::vk