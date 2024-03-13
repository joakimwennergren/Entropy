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
            class Quad
            {
            public:
                /**
                 * @brief Construct a new Quad object
                 *
                 * @param context
                 */
                Quad(std::shared_ptr<ServiceLocator> serviceLocator);
                ~Quad();

                VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
                VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;

                std::unique_ptr<Texture> _blank;

                std::unique_ptr<VertexBuffer> vertexBuffer;
                std::unique_ptr<IndexBuffer<uint16_t>> indexBuffer;

                void UpdateDescriptorSets();

            private:
                VkSampler _textureSampler;
                std::shared_ptr<ServiceLocator> _serviceLocator;
            };
        }
    }
}
