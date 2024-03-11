#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <config.hpp>

namespace Entropy
{
    namespace Graphics
    {
        namespace ValidationLayers
        {
            class ValidationLayer
            {
            public:
                static bool CheckValidationLayerSupport(const std::vector<const char *> validationLayers);
            };
        }
    }
}