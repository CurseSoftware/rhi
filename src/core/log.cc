#include "../../include/rhi/core/log.h"

namespace rhi::log
{
    struct log_info
    {
        std::ostream* out { &std::cout };
        std::ostream* err { &std::cerr };
    } g_log_info {};

    void log_initializer::init() const noexcept
    {
        g_log_info.out = _detail.out;
        g_log_info.err = _detail.err;
    }

    log_initializer& log_initializer::set_out(std::ostream& out) noexcept
    {
        _detail.out = &out;
        return *this;
    }

    log_initializer& log_initializer::set_err(std::ostream& err) noexcept
    {
        _detail.err = &err;
        return *this;
    }

    void info_impl(const std::string_view message) noexcept
    {
        (*g_log_info.out) << "[INFO]  " << message << '\n';
    }

    void trace_impl(const std::string_view message) noexcept
    {
        (*g_log_info.out) << "[TRACE] " << message << '\n';
    }

    void debug_impl(const std::string_view message) noexcept
    {
        (*g_log_info.out) << "[DEBUG] " << message << '\n';
    }

    void warn_impl(const std::string_view message) noexcept
    {
        (*g_log_info.out) << "[WARN]  " << message << '\n';
    }

    void error_impl(const std::string_view message) noexcept
    {
        (*g_log_info.err) << "[ERROR] " << message << '\n';
    }

    void fatal_impl(const std::string_view message) noexcept
    {
        (*g_log_info.err) << "[FATAL] " << message << '\n';
    }
}