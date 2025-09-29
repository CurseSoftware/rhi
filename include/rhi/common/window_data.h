//
// Created by ralex on 9/28/2025.
//

#ifndef RHI_WINDOW_DATA_H
#define RHI_WINDOW_DATA_H

#include "core/defines.h"
#include "core/win32.h"

namespace rhi
{
#ifdef RHI_PLATFORM_WINDOWS
    struct window_data_win32
    {
        HWND hwnd;
    };

#elif defined(RHI_PLATFORM_LINUX)
    struct window_data_x11
    {
    };
#endif


    using window_data = std::variant<
#ifdef RHI_PLATFORM_WINDOWS
        window_data_win32
#if defined(RHI_PLATFORM_LINUX)
        window_data_x11
#endif
    >;

} // rhi namespace

#endif
#endif