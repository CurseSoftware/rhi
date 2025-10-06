//
// Created by ralex on 10/5/2025.
//

#ifndef RHI_FORMAT_H
#define RHI_FORMAT_H
#include <format>
#include <string>

namespace rhi
{
    template <typename... Args>
    std::string format_str(const std::format_string<Args...> format_template, Args... args)
    {
        return std::vformat(format_template.get(), std::make_format_args(args...));
    }
} // rhi namespace

#endif //RHI_FORMAT_H