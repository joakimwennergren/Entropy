#pragma once

#include <data/vertex.hpp>
#include <filesystem/filesystem.hpp>
#include <graphics/vulkan/buffers/indexbuffer.hpp>
#include <graphics/vulkan/buffers/vertexbuffer.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>

// using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Data;

namespace Entropy {
  namespace Graphics {
    namespace Primitives {
      class Plane {
      public:
        /**
         * @brief Construct a new Plane object
         *
         * @param context
         */
        Plane();

        void Test() {
        };

        VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
        VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;

        // std::unique_ptr<Texture> _blank;

        void UpdateDescriptorSets();

        /**
         * @brief Destroy the Plane object
         *
         */
        ~Plane();

        std::unique_ptr<Vulkan::Buffers::VertexBuffer> vertexBuffer;
        std::unique_ptr<Vulkan::Buffers::IndexBuffer<uint16_t> > indexBuffer;

      private:
        VkSampler _textureSampler;
      };
    } // namespace Primitives
  } // namespace Graphics
} // namespace Entropy
