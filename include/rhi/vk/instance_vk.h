//
// Created by ralex on 9/28/2025.
//

#ifndef RHI_INSTANCE_VK_H
#define RHI_INSTANCE_VK_H
#include <optional>
#include <vector>
#include <string>

#include "core/expected.h"
#include "common/error.h"
#include "common/window_data.h"

namespace rhi::vk
{
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

        private:
            struct
            {
                bool enable_debug { false };
                std::vector<std::string> extensions {};
                std::optional<window_data> window {};
            } _info {};
        };

    public:
        instance(const instance&) noexcept = default;
        instance& operator=(const instance&) noexcept = default;
        instance(instance&&) = delete;
        instance& operator=(instance&) = delete;

    private:
        [[nodiscard]] explicit instance() = default;
    };
} // namespace rhi::vk

#endif //RHI_INSTANCE_VK_H