#include "vk/instance_vk.h"

#include <vulkan/vulkan_core.h>

#include "vk/core/extension_handler.h"

namespace rhi::vk
{
    expected<instance, error> instance::builder::build() const noexcept
    {
        instance inst {};

        extension_handler instance_extension_handler {};
        instance_extension_handler.fetch_all_extensions<instance>();

        return ok(inst);
    }

} // namespace rhi::vk