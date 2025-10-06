//
// Created by ralex on 10/5/2025.
//

#include "vk/core/utils.h"
#include "vk/vulkan.h"

#include <exception>
#include <iostream>

namespace rhi::vk
{
    void vk_assert(const VkResult result, const char* msg) noexcept
    {
        if (result != VK_SUCCESS)
        {
            std::cerr << msg << std::endl;
            std::terminate();
        }
    }

    bool vk_check(VkResult result) noexcept
    {
        return result == VK_SUCCESS;
    }

} // rhi::namespace