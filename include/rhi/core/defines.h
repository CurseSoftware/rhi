//
// Created by ralex on 9/28/2025.
//

#ifndef RHI_DEFINES_H
#define RHI_DEFINES_H

// Platform Detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define RHI_PLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64 bit is required for windows"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
#define RHI_PLATFORM_LINUX 1
#elif defined(__unix__)
#define RHI_PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
#define RHI_PLATFORM_POSIX 1
#elif __APPLE__
#include <TargetConditionals.h>
#define RHI_PLATFORM_APPLE 1
#endif

#ifdef RHI_EXPORT
#ifdef _MSC_VER
#define RHI_API __declspec(dllexport)
#else
#define RHI_API __attribute__((visibility("default")))
#endif
#else
// Imports
#ifdef _MSC_VER
#define RHI_API __declspec(dllimport)
#else
#define RHI_API
#endif
#endif

#endif //RHI_DEFINES_H