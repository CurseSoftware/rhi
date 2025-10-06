#include <cstring>
#include <cstdint>
#include <iostream>

#include "core/expected.h"
#include "vk/core/validation_handler.h"
#include "vk/vulkan.h"

namespace rhi::vk
{
    validation_layer_handler::validation_layer_handler() noexcept
    {
        uint32_t layer_count = 0;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        _layers.resize(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, _layers.data());
    }

    bool validation_layer_handler::is_supported(const char* layer_name) const noexcept
    {
        for (const auto& layer : _layers)
        {
            if (strcmp(layer_name, layer.layerName) == 0)
            {
                return true;
            }
        }

        return false;
    }

    expected<std::vector<const char*>, const char*> validation_layer_handler::get_requested(std::span<requested_layer> requested_layers) const noexcept
    {
        std::vector<const char*> found_layers;

        for (const auto& layer : requested_layers)
        {
            if (!is_supported(layer.layer_name))
            {
                if (layer.required)
                {
                    std::cout << "Required layer " << layer.layer_name << " is missing." << std::endl;
                    return unexpected(layer.layer_name);
                }
            }
            else
            {
                std::cout << "Found extension " << layer.layer_name << std::endl;
                found_layers.push_back(layer.layer_name);
            }
        }

        std::cout << "Found all required extensions" << std::endl;

        return ok(found_layers);
    }


} // namespace rhi::vk