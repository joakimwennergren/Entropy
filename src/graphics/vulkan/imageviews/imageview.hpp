#pragma once

#include <vulkan/vulkan.hpp>

#include <graphics/vulkan/queuefamilies/queuefamily.hpp>
#include <graphics/vulkan/surfaces/surface.hpp>
#include <graphics/vulkan/devices/ilogical_device.hpp>


using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::QueueFamilies;

namespace Entropy::Graphics::Vulkan::ImageViews {
  class ImageView {
    /**
     * Constructs an ImageView object for a given VkImage and VkFormat.
     *
     * @param image The Vulkan image handle (VkImage) for which the view is created.
     * @param format The format of the Vulkan image (VkFormat) that the view will use.
     *
     * @return A new instance of the ImageView class.
     */
  public:
    ImageView(VkImage image, VkFormat format);

    /**
     * Constructs an ImageView object for a given VkImage, VkFormat, and aspect mask flags.
     *
     * @param depthImage The Vulkan image handle (VkImage) for which the view is created.
     * @param format The format of the Vulkan image (VkFormat) that the view will use.
     * @param flags The aspect mask flags (uint32_t) that specify the purpose of this image view.
     *
     * @return A new instance of the ImageView class.
     */
    ImageView(VkImage depthImage, VkFormat format,
              uint32_t flags);

    [[nodiscard]] VkImageView Get() const { return _imageView; }

  private:
    VkImageView _imageView;
  };
} // namespace Entropy::Graphics::Vulkan::ImageViews
