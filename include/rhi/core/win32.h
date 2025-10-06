//
// Created by ralex on 9/28/2025.
//

#ifndef RHI_WIN32_H
#define RHI_WIN32_H

#include "core/defines.h"

#ifdef RHI_PLATFORM_WINDOWS

#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX
#include <windows.h>

#endif // RHI_PLATFORM_WINDOWS

#endif //RHI_WIN32_H