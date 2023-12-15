#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>
#include <string>
#include <config.hpp>

#include <graphics/surfaces/surface.hpp>
#include <graphics/queuefamilies/queuefamily.hpp>
#include <graphics/devices/logical_device.hpp>

using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::QueueFamilies;
using namespace Entropy::Graphics::Devices;

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
                ImageView(std::shared_ptr<LogicalDevice> logicalDevice, VkImage image, VkFormat format);

                inline VkImageView Get() { return _imageView; };

            private:
                VkImageView _imageView;
            };
        }
    }
}