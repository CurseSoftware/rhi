//
// Created by ralex on 9/28/2025.
//

#ifndef RHI_VULKAN_H
#define RHI_VULKAN_H
#include <exception>
#include <stdexcept>

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

template <typename T>
class vulkan_object
{
public:
    virtual ~vulkan_object() = default;

    /// Getter for the vulkan object handle.
    virtual auto get() noexcept -> T { return _handle; }

    /// Destroy the vulkan object.
    /// Note that not every object handler is
    /// responsible for its own destruction,
    /// so this does not need to be implemented
    /// for each type.
    ///
    /// This triggers an exception if this is called
    virtual auto destroy() -> void
    {
        throw new std::runtime_error("vulkan_object<T>::destroy Called on object that did not define it. If it is meant to define it, then do so. Otherwise this object is not supposed to handle its own destruction.");
    };

protected:
    /// Handle to the vulkan object
    T _handle;
};

#endif

#endif //RHI_VULKAN_H