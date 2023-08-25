#pragma once

#include <string>

#include "context.hpp"
#include "buffer.hpp"
#include "commandbuffer.hpp"

using namespace Symbios::Core;
using namespace Symbios::Graphics::Buffers;
using namespace Symbios::Graphics::CommandBuffers;

namespace Symbios
{
    namespace Graphics
    {
        namespace Textures
        {
            class Texture
            {
            public:
                Texture() = default;

                Texture(std::shared_ptr<Context> context);

                ~Texture();

                void CreateTextureImage(std::string path);

                void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);

                /**
                 * @brief Get the Image View object
                 *
                 * @return VkImageView
                 */
                inline VkImageView GetImageView() { return this->_imageView; };

            private:
                void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
                void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
                std::shared_ptr<Context> _context;
                std::unique_ptr<CommandBuffer> _commandBuffer;
                VkBuffer _stagingBuffer;
                VkDeviceMemory _stagingBufferMemory;
                VkImage _textureImage;
                VkImageView _imageView;
                VkDeviceMemory _textureImageMemory;
            };
        }
    }
}