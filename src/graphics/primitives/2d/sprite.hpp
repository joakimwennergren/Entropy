#pragma once

#include <graphics/data/vertex.hpp>
#include <graphics/textures/texture.hpp>
#include <graphics/buffers/vertexbuffer.hpp>
#include <renderables/renderable.hpp>
#include <filesystem/filesystem.hpp>
#include <scripting/script.hpp>

using namespace Entropy::Graphics::Textures;
using namespace Entropy::Renderables;
using namespace Entropy::Scripting;

namespace Entropy
{
    namespace Graphics
    {
        namespace Primitives
        {
            class Sprite : public Renderable
            {
            public:
                Sprite();

                Sprite(std::shared_ptr<ServiceLocator> serviceLocator, FT_Bitmap bitmap);

                Sprite(std::shared_ptr<ServiceLocator> serviceLocator, std::string path);

                Sprite(unsigned char *pixels, int width, int height);

                VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
                VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;

            private:
                void UpdateDescriptorSets();
            };
        }
    }
}
