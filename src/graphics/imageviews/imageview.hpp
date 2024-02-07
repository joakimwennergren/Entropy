#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>
#include <string>
#include <config.hpp>

#include <graphics/surfaces/surface.hpp>
#include <graphics/queuefamilies/queuefamily.hpp>

using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::QueueFamilies;

namespace Entropy
{
    namespace Graphics
    {
        namespace ImageViews
        {
            class ImageView
            {
            public:
                ImageView() = default;
                ImageView(VkDevice logicalDevice, VkImage image, VkFormat format);
                ImageView(VkDevice logicalDevice, VkImage depthImage, VkFormat format, uint32_t flags);

                inline VkImageView Get() { return _imageView; };

            private:
                VkImageView _imageView;
            };
        }
    }
}