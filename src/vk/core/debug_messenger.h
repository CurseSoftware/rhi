//
// Created by ralex on 10/9/2025.
//

#ifndef RHI_DEBUG_MESSENGER_H
#define RHI_DEBUG_MESSENGER_H

#include <string>

#include "core/expected.h"
#include "vk/vulkan.h"

namespace rhi::vk
{
    class debug_messenger
    {
    public:
        ~debug_messenger() noexcept = default;
        debug_messenger(debug_messenger const&) = default;
        debug_messenger(debug_messenger &&) = default;

        [[nodiscard]] debug_messenger& operator=(debug_messenger const&) = default;
        [[nodiscard]] debug_messenger& operator=(debug_messenger &&) = default;

        [[nodiscard]] static expected<debug_messenger, std::string> create(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT&) noexcept;

        [[nodiscard]] static VkDebugUtilsMessengerCreateInfoEXT get_create_info() noexcept;

        void destroy() noexcept;

    private:
        [[nodiscard]] explicit debug_messenger() noexcept = default;

    private:
        VkInstance _instance { VK_NULL_HANDLE };
        VkDebugUtilsMessengerEXT _debug_messenger { VK_NULL_HANDLE };
    };
} // namespace rhi::vk

#endif //RHI_DEBUG_MESSENGER_H