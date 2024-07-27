#pragma once
#include "../../graphics/vulkan/memory/allocator.hpp"
#include "../../graphics/vulkan/synchronization/queuesync.hpp"
#include "../../graphics/vulkan/textures/depthbuffer_texture.hpp"
#include "../../graphics/vulkan/textures/swapchain_texture.hpp"
#include "../../graphics/vulkan/textures/texture.hpp"
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

        TextureFactory() {}

        std::shared_ptr<DepthBufferTexture>
        CreateDepthBufferTexture(unsigned int width, unsigned int height)
        {
          return std::make_shared<DepthBufferTexture>(width, height);
        }

        std::shared_ptr<SwapChainTexture>
        CreateSwapChainTexture(unsigned int width, unsigned int height)
        {
          return std::make_shared<SwapChainTexture>(width, height);
        }

        std::shared_ptr<NormalTexture> CreateNormalTexture(std::string path)
        {
          return std::make_shared<NormalTexture>(path);
        }

      private:
        // Vulkan Dependency
      };

    } // namespace Vulkan
  } // namespace Factories
} // namespace Entropy