//
// Created by ralex on 10/5/2025.
//

#ifndef RHI_QUEUE_FAMILY_INDICES_H
#define RHI_QUEUE_FAMILY_INDICES_H
#include <cstdint>
#include <optional>

#include "vk/core/physical_device_handler.h"

namespace rhi::vk
{
    class queue_family_indices
    {
    public:
        [[nodiscard]] queue_family_indices(VkPhysicalDevice, VkSurfaceKHR) noexcept;

        [[nodiscard]] bool is_complete() const noexcept
        {
            return _graphics_family_index.has_value() && _present_family_index.has_value();
        }

        [[nodiscard]] uint32_t get_graphics() const noexcept { return _graphics_family_index.value(); }
        [[nodiscard]] uint32_t get_present() const noexcept { return _present_family_index.value(); }

    private:
        std::optional<uint32_t> _graphics_family_index { std::nullopt };
        std::optional<uint32_t> _present_family_index  { std::nullopt };
    };
} // namespace rhi::vk

#endif //RHI_QUEUE_FAMILY_INDICES_H