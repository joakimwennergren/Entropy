#pragma once

#include <cstdint>

#include "graphics/vulkan/textures/texture.hpp"
#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

#include <data/vertex.hpp>
#include <graphics/vulkan/buffers/indexbuffer.hpp>
#include <graphics/vulkan/buffers/vertexbuffer.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <scripting/script.hpp>


using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Scripting;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Data;

namespace Entropy::Graphics::Primitives {
  struct Sprite {
    explicit Sprite(const std::string &path) {
      texture = std::make_shared<Texture>(path);
      vertexBuffer = std::make_shared<VertexBuffer>(vertices);
      indexBuffer = std::make_shared<IndexBuffer<uint16_t> >(indices);
    }

#ifdef BUILD_FOR_ANDROID
  Sprite(std::string path, AAssetManager *assetmanager);
#endif

    std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};
    std::vector<Vertex> vertices = {
      {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
      {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
      {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
      {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };
    std::shared_ptr<Texture> texture;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer<uint16_t> > indexBuffer;
  };
} // namespace Entropy::Graphics::Primitives
