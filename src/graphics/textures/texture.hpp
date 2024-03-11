#pragma once

#include <string>

#include <spdlog/spdlog.h>

#include <stb_image.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "config.hpp"
#include "tiny_gltf.h"

#include <graphics/buffers/buffer.hpp>
#include <graphics/commandbuffers/commandbuffer.hpp>
#include <graphics/utilities/utilities.hpp>
#include <graphics/buffers/stagedbuffer.hpp>
#include <graphics/imageviews/imageview.hpp>
#include <graphics/memory/allocator.hpp>
#include <graphics/synchronization/queuesync.hpp>

// #include <ktx.h>
// #include <ktxvulkan.h>

#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

using namespace Entropy::Graphics::Buffers;
using namespace Entropy::Graphics::Utilities;
using namespace Entropy::Graphics::CommandBuffers;
using namespace Entropy::Graphics::ImageViews;
using namespace Entropy::Graphics::Memory;
using namespace Entropy::Graphics::Synchronization;

namespace Entropy
{
    namespace Graphics
    {
        namespace Textures
        {
            class Texture
            {
            public:
                Texture(std::shared_ptr<ServiceLocator> serviceLocator);
                ~Texture();

                void CreateTextureImage(std::string path);

                void CreateTextureImageFromPixels(unsigned char *pixels, int width, int height);

                // void CreateTextureImageFromKtx(unsigned char *pixels, unsigned int width, unsigned int height, int size, int mips, VkFormat format, ktxTexture *ktxTexture);

                void CreateTextureImageFromBuffer(FT_Bitmap bitmap);

                void CreateTextureFromGLTFImage(tinygltf::Image &gltfimage);

#ifdef BUILD_FOR_ANDROID
                void CreateTextureImage(std::string path, AAssetManager *assetManager);
#endif

                inline VkImageView GetImageView() { return this->_imageView; };

            private:
                void TransitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout) const;
                void CopyBufferToImage(const VkBuffer buffer, const VkImage image, const uint32_t width, const uint32_t height) const;
                void CreateImage(const uint32_t width, const uint32_t height, const VkFormat format, const VkImageTiling tiling, const VkImageUsageFlags usage, VkImage &image);
                VkFormat GetColorFormat();

                VkImage _textureImage = VK_NULL_HANDLE;
                VkImageView _imageView = VK_NULL_HANDLE;
                VmaAllocation _allocation = VK_NULL_HANDLE;
                std::shared_ptr<ServiceLocator> _serviceLocator;
                std::shared_ptr<LogicalDevice> _logicalDevice;
                std::shared_ptr<QueueSync> _queueSync;
                std::shared_ptr<Allocator> _allocator;
            };
        }
    }
}
