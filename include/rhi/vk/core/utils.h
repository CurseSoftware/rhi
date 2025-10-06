//
// Created by ralex on 10/5/2025.
//

#ifndef RHI_UTILS_H
#define RHI_UTILS_H

#include "vk/vulkan.h"

namespace rhi::vk
{
    void vk_assert(VkResult result, const char* msg) noexcept;

    bool vk_check(VkResult result) noexcept;
} // namespace rhi::vk

#endif //RHI_UTILS_H