#pragma once

#include <graphics/data/vertex.hpp>
#include <graphics/textures/texture.hpp>
#include <graphics/buffers/vertexbuffer.hpp>
#include <renderables/renderable.hpp>
#include <filesystem/filesystem.hpp>

using namespace Entropy::Graphics::Textures;
using namespace Entropy::Renderables;

namespace Entropy
{
    namespace Graphics
    {
        namespace Primitives
        {
            class Plane : public Renderable
            {
            public:
                /**
                 * @brief Construct a new Plane object
                 *
                 * @param context
                 */
                Plane(std::shared_ptr<ServiceLocator> serviceLocator);

                void Test(){};

                VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
                VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;

                std::unique_ptr<Texture> _blank;

                void UpdateDescriptorSets();
                /**
                 * @brief Destroy the Plane object
                 *
                 */
                ~Plane();
            };
        }
    }
}
