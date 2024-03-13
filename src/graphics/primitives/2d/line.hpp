#pragma once

#include <graphics/data/vertex.hpp>
#include <graphics/textures/texture.hpp>
#include <graphics/buffers/vertexbuffer.hpp>
#include <graphics/buffers/indexbuffer.hpp>
#include <graphics/descriptorpools/descriptorpool.hpp>
#include <filesystem/filesystem.hpp>

using namespace Entropy::Graphics::Textures;
using namespace Entropy::Graphics::Buffers;
using namespace Entropy::Graphics::DescriptorPools;

namespace Entropy
{
    namespace Graphics
    {
        namespace Primitives
        {
            class Line
            {
            public:
                /**
                 * @brief Construct a new Quad object
                 *
                 * @param context
                 */
                Line(std::shared_ptr<ServiceLocator> serviceLocator, glm::vec3 start, glm::vec3 end, glm::vec4 color);

                VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
                VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;

                std::unique_ptr<Texture> _blank;
                std::unique_ptr<VertexBuffer> vertexBuffer;
                std::unique_ptr<IndexBuffer<uint16_t>> indexBuffer;
                void UpdateDescriptorSets();
                /**
                 * @brief Destroy the Quad object
                 *
                 */
                ~Line();

            private:
                VkSampler _textureSampler;
                std::shared_ptr<ServiceLocator> _serviceLocator;
            };
        }
    }
}
