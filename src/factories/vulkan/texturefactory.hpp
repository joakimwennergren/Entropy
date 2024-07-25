#pragma once
#include "../../graphics/vulkan/memory/allocator.hpp"
#include "../../graphics/vulkan/synchronization/queuesync.hpp"
#include "../../graphics/vulkan/textures/depthbuffer_texture.hpp"
#include "../../graphics/vulkan/textures/swapchain_texture.hpp"
#include "../../graphics/vulkan/textures/texture.hpp"
#include "../../graphics/vulkan/vulkan_backend.hpp"
#include "factories/vulkan/bufferfactory.hpp"
#include "graphics/vulkan/commandpools/commandpool.hpp"
#include "graphics/vulkan/textures/normal_texture.hpp"

using namespace Entropy::Graphics::Vulkan;
using namespace Entropy::Graphics::Vulkan::Textures;

namespace Entropy
{
  namespace Factories
  {
    namespace Vulkan
    {

      struct TextureFactory
      {

        TextureFactory(VulkanBackend backend, QueueSync qs, Allocator a,
                       BufferFactory bf, CommandPool cp, DescriptorPool dp)
            : _vkBackend{backend}, _queueSync{qs}, _allocator{a}, _bufferFactory{bf},
              _commandPool{cp}, _descriptorPool{dp} {}

        std::shared_ptr<DepthBufferTexture>
        CreateDepthBufferTexture(unsigned int width, unsigned int height)
        {
          return std::make_shared<DepthBufferTexture>(
              _vkBackend, _queueSync, _allocator, _bufferFactory, _commandPool,
              _descriptorPool, width, height);
        }

        std::shared_ptr<SwapChainTexture>
        CreateSwapChainTexture(unsigned int width, unsigned int height)
        {
          return std::make_shared<SwapChainTexture>(
              _vkBackend, _queueSync, _allocator, _bufferFactory, _commandPool,
              _descriptorPool, width, height);
        }

        std::shared_ptr<NormalTexture> CreateNormalTexture(std::string path)
        {
          return std::make_shared<NormalTexture>(_vkBackend, _queueSync, _allocator,
                                                 _bufferFactory, _commandPool,
                                                 _descriptorPool, path);
        }
        Allocator _allocator;

      private:
        // Vulkan Dependency
        VulkanBackend _vkBackend;
        QueueSync _queueSync;

        BufferFactory _bufferFactory;
        CommandPool _commandPool;
        DescriptorPool _descriptorPool;
      };

    } // namespace Vulkan
  } // namespace Factories
} // namespace Entropy