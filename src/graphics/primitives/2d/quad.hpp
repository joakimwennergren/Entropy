#pragma once

#include "graphics/vulkan/textures/normal_texture.hpp"
#include <data/vertex.hpp>
#include <filesystem/filesystem.hpp>
#include <graphics/vulkan/buffers/indexbuffer.hpp>
#include <graphics/vulkan/buffers/vertexbuffer.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/textures/texture.hpp>

using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Data;

namespace Entropy
{
  namespace Graphics
  {
    namespace Primitives
    {
      struct Quad
      {
      public:
        Quad(std::vector<Vertex> vertices)
        {
          // vertexBuffer = bufferFactory.CreateVertexBuffer(vertices);
          // indexBuffer = bufferFactory.CreateIndexBuffer<uint16_t>(indices);
          // this->vertices = vertices;
          // this->texture = textureFactory.CreateNormalTexture(
          //     Filesystem::GetSpritesDir() + "_blank.png");
        }
        std::shared_ptr<IndexBuffer<uint16_t>> indexBuffer;
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::vector<Vertex> vertices;
        // Indices for the two triangles that make up the quad
        std::vector<uint16_t> indices = {
            0, 1, 2, // First triangle
            2, 3, 0  // Second triangle
        };

        std::shared_ptr<NormalTexture> texture;
      };
    } // namespace Primitives
  } // namespace Graphics
} // namespace Entropy
