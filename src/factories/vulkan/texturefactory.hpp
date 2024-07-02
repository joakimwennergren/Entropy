#pragma once
#include "../../graphics/vulkan/memory/allocator.hpp"
#include "../../graphics/vulkan/synchronization/queuesync.hpp"
#include "../../graphics/vulkan/textures/depthbuffer_texture.hpp"
#include "../../graphics/vulkan/textures/swapchain_texture.hpp"
#include "../../graphics/vulkan/textures/texture.hpp"
#include "../../graphics/vulkan/vulkan_backend.hpp"

using namespace Entropy::Graphics::Vulkan;
using namespace Entropy::Graphics::Vulkan::Textures;

namespace Entropy {
namespace Factories {
namespace Vulkan {

struct TextureFactory {

  TextureFactory(VulkanBackend backend, QueueSync qs, Allocator a)
      : _vkBackend{backend}, _queueSync{qs}, _allocator{a} {}

  DepthBufferTexture *CreateDepthBufferTexture(unsigned int width,
                                               unsigned int height) {
    return new DepthBufferTexture(_vkBackend, _queueSync, _allocator, width,
                                  height);
  }

  SwapChainTexture CreateSwapChainTexture(unsigned int width,
                                          unsigned int height) {
    return SwapChainTexture(_vkBackend, _queueSync, _allocator, width, height);
  }

private:
  // Vulkan Dependency
  VulkanBackend _vkBackend;
  QueueSync _queueSync;
  Allocator _allocator;
};

} // namespace Vulkan
} // namespace Factories
} // namespace Entropy