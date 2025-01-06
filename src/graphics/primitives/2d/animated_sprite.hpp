#pragma once

#include "graphics/vulkan/textures/normal_texture.hpp"
#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

#include <data/vertex.hpp>
#include <filesystem/filesystem.hpp>
#include <graphics/vulkan/buffers/indexbuffer.hpp>
#include <graphics/vulkan/buffers/vertexbuffer.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/textures/texture.hpp>
#include <scripting/script.hpp>

using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Scripting;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Data;

namespace Entropy
{
  namespace Graphics
  {
    namespace Primitives
    {

      struct AnimatedSprite
      {
      public:
        AnimatedSprite(std::vector<std::string> paths)
        {

          // for (auto path : paths)
          // {
          //   textures.push_back(textureFactory.CreateNormalTexture(
          //       Filesystem::GetSpritesDir() + path));
          // }

          // vertexBuffer = bufferFactory.CreateVertexBuffer(vertices);
          // indexBuffer = bufferFactory.CreateIndexBuffer(indices);
        }

        std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};
        std::vector<Vertex> vertices = {{{-1.0f, -1.0f, 0.0f},
                                         {1.0f, 1.0f, 1.0f},
                                         {1.0f, 0.0f},
                                         {0.0, 0.0},
                                         {0.0, 0.0, 0.0, 0.0},
                                         {0.0, 0.0, 0.0, 0.0},
                                         {0.0, 1.0, 0.0, 0.0}},
                                        {{1.0f, -1.0f, 0.0f},
                                         {1.0f, 1.0f, 1.0f},
                                         {0.0f, 0.0f},
                                         {0.0, 0.0},
                                         {0.0, 0.0, 0.0, 0.0},
                                         {0.0, 0.0, 0.0, 0.0},
                                         {0.0, 1.0, 0.0, 0.0}},
                                        {{1.0f, 1.0f, 0.0f},
                                         {1.0f, 1.0f, 1.0f},
                                         {0.0f, 1.0f},
                                         {0.0, 0.0},
                                         {0.0, 0.0, 0.0, 0.0},
                                         {0.0, 0.0, 0.0, 0.0},
                                         {0.0, 1.0, 0.0, 0.0}},
                                        {{-1.0f, 1.0f, 0.0f},
                                         {1.0f, 1.0f, 1.0f},
                                         {1.0f, 1.0f},
                                         {0.0, 0.0},
                                         {0.0, 0.0, 0.0, 0.0},
                                         {0.0, 0.0, 0.0, 0.0},
                                         {0.0, 1.0, 0.0, 0.0}}};

        std::vector<std::shared_ptr<NormalTexture>> textures;
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<IndexBuffer<uint16_t>> indexBuffer;
      };
    } // namespace Primitives
  } // namespace Graphics
} // namespace Entropy
