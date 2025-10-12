//
// Created by ralex on 9/28/2025.
//

#ifndef RHI_INSTANCE_VK_H
#define RHI_INSTANCE_VK_H
#include <memory>
#include <optional>
#include <vector>
#include <string>

#include "core/expected.h"
#include "common/error.h"
#include "vk/vulkan.h"
#include "common/window_data.h"
#include "vk/core/physical_device_handler.h"
#include "vk/core/debug_messenger.h"

namespace rhi::vk
{
    enum class surface_create_error
    {
        INVALID_WINDOW_DATA_NOT_WIN32,
        INVALID_WINDOW_DATA_NOT_XLIB,
        INVALID_WINDOW_DATA_NOT_METAL,
        INVALID_WINDOW_DATA_NOT_WAYLAND,
        CREATE_SURFACE_FAILED
    };

    class instance
    {
    public:
        class builder
        {
        public:
            [[nodiscard]] expected<instance, error> build() const noexcept;

            [[nodiscard]] builder& enable_debug() noexcept
            {
                _info.enable_debug = true;
                return *this;
            }

            [[nodiscard]] builder& disable_debug() noexcept
            {
                _info.enable_debug = false;
                return *this;
            }

            [[nodiscard]] builder& add_extension(const std::string& ext) noexcept
            {
                _info.extensions.push_back(ext);
                return *this;
            }

            [[nodiscard]] builder& add_extensions(const std::vector<const char*>& exts) noexcept
            {
                for (const auto& ext : exts)
                {
                    _info.extensions.emplace_back(ext);
                }
                return *this;
            }

            [[nodiscard]] builder& set_window_data(const window_data& window) noexcept
            {
                _info.window = window;
                return *this;
            }

            [[nodiscard]] builder& headless(const bool headless = true) noexcept
            {
                _info.headless = headless;
                return *this;
            }

        private:
            struct
            {
                bool enable_debug { false };
                std::vector<std::string> extensions {};
                std::optional<window_data> window {};
                bool headless { false };
            } _info {};
        };

    public:
        instance(const instance& other) noexcept = delete;
        instance& operator=(const instance&) noexcept = delete;
        instance(instance&&) = default;
        instance& operator=(instance&&) noexcept = default;

        // instance(instance&& other) noexcept
        // {
        //     _detail = std::move(other._detail);
        //     _debug_messenger = std::move(other._debug_messenger);
        //     _suitable_devices = std::move(other._suitable_devices);
        // }
        // instance& operator=(instance&& other) noexcept
        // {
        //     _detail = std::move(other._detail);
        //     _debug_messenger = std::move(other._debug_messenger);
        //     _suitable_devices = std::move(other._suitable_devices);
        //     return *this;
        // }

        expected<VkSurfaceKHR, surface_create_error> create_surface(const window_data&) const noexcept;

        void destroy() noexcept;

    private:
        [[nodiscard]] explicit instance() = default;

    private:

        struct
        {
            VkInstance instance { VK_NULL_HANDLE };
            VkSurfaceKHR surface { VK_NULL_HANDLE };
        } _detail {};

        std::vector<class physical_device> _suitable_devices {};
        std::unique_ptr<debug_messenger> _debug_messenger { nullptr };
    };

    static_assert(std::is_move_assignable_v<instance> && "Instance not move assignable");
    static_assert(std::is_move_constructible_v<instance> && "Instance not move constructible");
    static_assert(std::is_copy_assignable_v<instance> == false && "Instance not copy assignable");
    static_assert(std::is_copy_constructible_v<instance> == false && "Instance not copy constructible");
} // namespace rhi::vk

#endif //RHI_INSTANCE_VK_H