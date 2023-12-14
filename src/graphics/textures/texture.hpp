#pragma once

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "config.hpp"

#include <graphics/buffers/buffer.hpp>
#include <graphics/commandbuffers/commandbuffer.hpp>
#include <graphics/utilities/utilities.hpp>
#include <graphics/buffers/stagedbuffer.hpp>

using namespace Entropy::Global;
using namespace Entropy::Graphics::Buffers;
using namespace Entropy::Graphics::Utilities;
using namespace Entropy::Graphics::CommandBuffers;

namespace Entropy
{
    namespace Graphics
    {
        namespace Textures
        {
            class Texture
            {
            public:
                /**
                 * @brief Construct a new Texture object
                 *
                 * @param context
                 */
                Texture();

                /**
                 * @brief Destroy the Texture object
                 *
                 */
                ~Texture();

                /**
                 * @brief Create a Texture Image object
                 *
                 * @param path
                 */
                void CreateTextureImage(std::string path);

                /**
                 * @brief Create a Texture Image object
                 *
                 * @param path
                 */
                void CreateTextureImageFromPixels(unsigned char *pixels, int width, int height);

                /**
                 *
                 */
                void CreateTextureImageFromBuffer(FT_Bitmap bitmap);

                /**
                 * @brief Get the Image View object
                 *
                 * @return VkImageView
                 */
                inline VkImageView GetImageView() { return this->_imageView; };

                bool hasTexture = false;

            private:
                /**
                 * @brief
                 *
                 * @param image
                 * @param format
                 * @param oldLayout
                 * @param newLayout
                 */
                void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

                /**
                 * @brief
                 *
                 * @param buffer
                 * @param image
                 * @param width
                 * @param height
                 */
                void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

                /**
                 * @brief Create an Image object
                 *
                 * @param width
                 * @param height
                 * @param format
                 * @param tiling
                 * @param usage
                 * @param properties
                 * @param image
                 * @param imageMemory
                 */
                void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);

                std::unique_ptr<CommandBuffer> _commandBuffer;

                VkImage _textureImage;
                VkImageView _imageView;
                VkDeviceMemory _textureImageMemory;
            };
        }
    }
}