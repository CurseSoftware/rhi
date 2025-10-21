//
// Created by ralex on 9/28/2025.
//

#ifndef RHI_DEVICE_VK_H
#define RHI_DEVICE_VK_H
#ifdef BACKEND_USE_VULKAN

#include "common/error.h"
#include "core/expected.h"
#include "vk/vulkan.h"
#include "vk/core/physical_device_handler.h"

#include <string>

namespace rhi::vk
{
    class device : public vulkan_object<VkDevice>
    {
    public:
        class builder
        {
        public:
            [[nodiscard]] explicit builder(const physical_device& device) noexcept
                : _info{ device }
            {}
        public:
            [[nodiscard]] expected<device, std::string> build() noexcept;

            [[nodiscard]] builder& request_extension(const char*) noexcept;
            [[nodiscard]] builder& request_extensions(const std::span<const char*>) noexcept;

            [[nodiscard]] builder& surface(const VkSurfaceKHR&) noexcept;

        private:
            struct
            {
                class physical_device physical_device;
                std::vector<const char*> extensions {};
                VkSurfaceKHR surface { VK_NULL_HANDLE };
            } _info {};
        };

        auto destroy() noexcept -> void override;

    private:
        // VkDevice _device { VK_NULL_HANDLE };
    };
} // namespace rhi::vk

#endif // BACKEND_USE_VULKAN

#endif //RHI_DEVICE_VK_H