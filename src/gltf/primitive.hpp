#pragma once

#include <glm/glm.hpp>
#include "pbr_material.hpp"

namespace Entropy
{
    namespace GLTF
    {
        struct Primitive
        {
            uint32_t firstIndex;
            uint32_t indexCount;
            uint32_t vertexCount;
            Material &material;
            bool hasIndices;
            BoundingBox bb;
            Primitive(uint32_t firstIndex, uint32_t indexCount, uint32_t vertexCount, Material &material);
            void setBoundingBox(glm::vec3 min, glm::vec3 max);
        };
    }
}