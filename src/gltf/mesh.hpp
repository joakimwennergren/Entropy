#pragma once

#include <glm/glm.hpp>
#include "primitive.hpp"

namespace Entropy
{
    namespace GLTF
    {
        struct Mesh
        {
            std::vector<Primitive *> primitives;
            BoundingBox bb;
            BoundingBox aabb;

            struct UniformBuffer
            {
                VkBuffer buffer;
                VkDeviceMemory memory;
                VkDescriptorBufferInfo descriptor;
                VkDescriptorSet descriptorSet;
                void *mapped;
            } uniformBuffer;

            struct UniformBlock
            {
                glm::mat4 matrix;
                glm::mat4 jointMatrix[MAX_NUM_JOINTS]{};
                float jointcount{0};
            } uniformBlock;

            Mesh(vks::VulkanDevice *device, glm::mat4 matrix);
            ~Mesh();

            void setBoundingBox(glm::vec3 min, glm::vec3 max);
        };
    }
}