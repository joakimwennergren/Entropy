#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include "primitive.hpp"
#include <servicelocators/servicelocator.hpp>
#include <graphics/devices/logical_device.hpp>
#include <graphics/buffers/uniformbuffer.hpp>

// Changing this value here also requires changing it in the vertex shader
#define MAX_NUM_JOINTS 128u

using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Devices;
using namespace Entropy::Graphics::Buffers;

namespace Entropy
{
    namespace GLTF
    {
        struct Mesh
        {
            std::vector<Primitive *> primitives;
            BoundingBox bb;
            BoundingBox aabb;

            struct UniformBlock
            {
                glm::mat4 matrix;
                glm::mat4 jointMatrix[MAX_NUM_JOINTS]{};
                float jointcount{0};
            } uniformBlock;

            Mesh(std::shared_ptr<ServiceLocator> serviceLocator, glm::mat4 matrix);
            ~Mesh();

            std::unique_ptr<UniformBuffer> _buffer;
            std::shared_ptr<LogicalDevice> _logicalDevice;

            void setBoundingBox(glm::vec3 min, glm::vec3 max);
        };
    }
}