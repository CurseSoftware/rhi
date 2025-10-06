//
// Created by ralex on 9/28/2025.
//

#ifndef RHI_VULKAN_H
#define RHI_VULKAN_H
#include "core/defines.h"

#ifdef BACKEND_USE_VULKAN

#ifdef RHI_PLATFORM_LINUX
#define VK_USE_PLATFORM_XLIB_KHR
#define VK_NO_PLATFORM_XCB_KHR
#elif defined(RHI_PLATFORM_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
// #include <vulkan/vulkan_win32.h>
#endif

#include <vulkan/vulkan.h>

#endif

#endif //RHI_VULKAN_H