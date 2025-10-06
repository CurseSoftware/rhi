//
// Created by ralex on 9/28/2025.
//

#ifndef RHI_EXTENSION_HANDLER_H
#define RHI_EXTENSION_HANDLER_H

#include <span>

#include "vk/vulkan.h"
#include "core/expected.h"

#include <string>
#include <string_view>
#include <vector>

namespace rhi::vk
{
    struct instance;
    struct physical_device;

    template <typename T>
    concept extension_source =
        std::is_same_v<T, instance>
        || std::is_same_v<T, physical_device>;

    struct requested_extension
    {
        bool is_required;
        std::string name;
    };

    class extension_handler
    {
    public:
        [[nodiscard]] explicit extension_handler() noexcept = default;

        template<extension_source S>
        void fetch_all_extensions() noexcept;

        expected<std::vector<const char*>, const char*> get_requested(std::span<requested_extension>) const noexcept;

        [[nodiscard]] bool is_supported(std::string_view) const noexcept;

    private:
        std::vector<VkExtensionProperties> _extensions;
    };
} // namespace rhi::vk

#endif //RHI_EXTENSION_HANDLER_H