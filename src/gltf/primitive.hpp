#pragma once

#include <glm/glm.hpp>
#include "materials/pbr_material.hpp"
#include <gltf/bounding_box.hpp>

using namespace Entropy::Materials;
using namespace Entropy::GLTF;

namespace Entropy
{
    namespace GLTF
    {
        struct Primitive
        {
            uint32_t firstIndex;
            uint32_t indexCount;
            int32_t materialIndex;
        };
    }
}