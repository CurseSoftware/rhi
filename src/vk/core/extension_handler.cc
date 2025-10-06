#include "vk/core/extension_handler.h"
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
                    std::cout << "Extension " << extension.name << " is missing." << std::endl;
                    return unexpected(extension.name.c_str());
                }
            }
            else
            {
                std::cout << "Found extension " << extension.name << std::endl;
                found_extensions.push_back(extension.name.c_str());
            }
        }
        std::cout << "Found all extensions\n";

        return ok(found_extensions);
    }



    bool extension_handler::is_supported(std::string_view extension_name) const noexcept
    {
        for (const auto& extension : _extensions)
        {
            if (strcmp(extension.extensionName, extension_name.data()) == 0)
            {
                return true;
            }
        }

        return false;
    }


} // namespace rhi::vk