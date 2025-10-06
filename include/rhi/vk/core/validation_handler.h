//
// Created by ralex on 10/1/2025.
//

#ifndef RHI_VALIDATION_HANDLER_H
#define RHI_VALIDATION_HANDLER_H
#include <span>
#include <vector>

#include "core/expected.h"
#include "vk/vulkan.h"

namespace rhi::vk
{
    using validation_layer = VkLayerProperties;
    struct requested_layer
    {
        bool required { true };
        const char* layer_name { "DEFAULT_LAYER_NAME" };
    };

    class validation_layer_handler
    {
    public:
        [[nodiscard]] validation_layer_handler() noexcept;

        [[nodiscard]] bool is_supported(const char*) const noexcept;

        [[nodiscard]] expected<std::vector<const char*>, const char*> get_requested(std::span<requested_layer>) const noexcept;

        [[nodiscard]] std::vector<validation_layer> get_layers() const noexcept { return _layers; }
    private:
        std::vector<validation_layer> _layers {};
    };
} // namespace rhi::vk

#endif //RHI_VALIDATION_HANDLER_H
