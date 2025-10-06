//
// Created by ralex on 10/5/2025.
//

#ifndef RHI_LOG_H
#define RHI_LOG_H
#include <format>
#include <iosfwd>
#include <iostream>
#include "core/format.h"

namespace rhi::log
{
    struct log_initializer
    {
        void init() const noexcept;
        [[nodiscard]] log_initializer& set_out(std::ostream& out) noexcept;
        [[nodiscard]] log_initializer& set_err(std::ostream& err) noexcept;

    private:
        struct
        {
            std::ostream* out { &std::cout };
            std::ostream* err { &std::cerr };
        } _detail;
    };

    void info_impl(std::string_view) noexcept;
    void trace_impl(std::string_view) noexcept;
    void debug_impl(std::string_view) noexcept;
    void warn_impl(std::string_view) noexcept;
    void error_impl(std::string_view) noexcept;
    void fatal_impl(std::string_view) noexcept;

    template <typename ...Args>
    void info(std::format_string<Args...> fmt, Args... args) noexcept
    {
        info_impl(format_str(fmt, args...));
    }

    template <typename ...Args>
    void trace(std::format_string<Args...> fmt, Args... args) noexcept
    {
        trace_impl(format_str(fmt, args...));
    }

    template <typename ...Args>
    void debug(std::format_string<Args...> fmt, Args... args) noexcept
    {
        debug_impl(format_str(fmt, args...));
    }

    template <typename ...Args>
    void warn(std::format_string<Args...> fmt, Args... args) noexcept
    {
        warn_impl(format_str(fmt, args...));
    }

    template <typename ...Args>
    void error(std::format_string<Args...> fmt, Args... args) noexcept
    {
        error_impl(format_str(fmt, args...));
    }

    template <typename ...Args>
    void fatal(std::format_string<Args...> fmt, Args... args) noexcept
    {
        fatal_impl(format_str(fmt, args...));
    }
} // namespace rhi::log

#endif //RHI_LOG_H