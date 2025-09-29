#include "vk/core/extension_handler.h"
#include "vk/vulkan.h"
#include <cstdint>
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