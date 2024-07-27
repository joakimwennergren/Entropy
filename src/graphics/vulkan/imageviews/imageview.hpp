#pragma once

#include <config.hpp>
#include <iostream>
#include <set>
#include <string>
#include <vulkan/vulkan.hpp>

#include <graphics/vulkan/queuefamilies/queuefamily.hpp>
#include <graphics/vulkan/surfaces/surface.hpp>

using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::QueueFamilies;

namespace Entropy
{
  namespace Graphics
  {
    namespace Vulkan
    {
      namespace ImageViews
      {
        class ImageView
        {
        public:
          ImageView() = default;
          ImageView(VkImage image, VkFormat format);
          ImageView(VkImage depthImage, VkFormat format,
                    uint32_t flags);
          inline VkImageView Get() { return _imageView; };

        private:
          VkImageView _imageView;
        };
      } // namespace ImageViews
    } // namespace Vulkan
  } // namespace Graphics
} // namespace Entropy