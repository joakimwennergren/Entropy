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

namespace Entropy {
namespace Factories {
namespace Vulkan {

struct TextureFactory {

  TextureFactory(VulkanBackend backend, QueueSync qs, Allocator a,
                 BufferFactory bf, CommandPool cp)
      : _vkBackend{backend}, _queueSync{qs}, _allocator{a}, _bufferFactory{bf},
        _commandPool{cp} {}

  DepthBufferTexture *CreateDepthBufferTexture(unsigned int width,
                                               unsigned int height) {
    return new DepthBufferTexture(_vkBackend, _queueSync, _allocator, width,
                                  height);
  }

  SwapChainTexture CreateSwapChainTexture(unsigned int width,
                                          unsigned int height) {
    return SwapChainTexture(_vkBackend, _queueSync, _allocator, width, height);
  }

  std::shared_ptr<NormalTexture> CreateNormalTexture(std::string path) {
    return std::make_shared<NormalTexture>(_vkBackend, _queueSync, _allocator,
                                           _bufferFactory, _commandPool, path);
  }

private:
  // Vulkan Dependency
  VulkanBackend _vkBackend;
  QueueSync _queueSync;
  Allocator _allocator;
  BufferFactory _bufferFactory;
  CommandPool _commandPool;
};

} // namespace Vulkan
} // namespace Factories
} // namespace Entropy