#ifndef ENTROPY_IMAGEVIEW_H
#define ENTROPY_IMAGEVIEW_H

#include <vulkan/vulkan.hpp>

#include <graphics/vulkan/queuefamilies/queuefamily.hpp>
#include <graphics/vulkan/surfaces/surface.hpp>
#include <graphics/vulkan/devices/ilogical_device.hpp>


namespace Entropy::Graphics::Vulkan::ImageViews {
    /**
     * Manages a view of a Vulkan image, allowing it to be accessed or manipulated.
     */
    struct ImageView {
        /**
         * Constructs an ImageView object for a given VkImage and VkFormat.
         *
         * @param image The Vulkan image handle (VkImage) for which the view is created.
         * @param format The format of the Vulkan image (VkFormat) that the view will use.
         *
         * @return A new instance of the ImageView class.
         */
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
        ImageView(VkImage image, VkFormat format,
                  uint32_t flags);

        /**
         * Destroys the ImageView object, releasing its associated resources.
         */
        ~ImageView();

        /**
         * Returns the Vulkan image view handle (VkImageView) associated with this ImageView instance.
         *
         * @return The Vulkan image view handle (VkImageView).
         */
        [[nodiscard]] VkImageView Get() const {
            return _imageView;
        }

    private:
        VkImageView _imageView{};
        std::shared_ptr<ILogicalDevice> _logicalDevice;
    };
} // namespace Entropy::Graphics::Vulkan::ImageViews

#endif // ENTROPY_IMAGEVIEW_H
