#pragma once

#include <ft2build.h>
#include <stb_image.h>
#include FT_FREETYPE_H

#include <spdlog/spdlog.h>
#include <string>

#include "tiny_gltf.h"
#include <config.hpp>

#include <assets/textureid.hpp>
#include <factories/vulkan/bufferfactory.hpp>
#include <graphics/vulkan/buffers/buffer.hpp>
#include <graphics/vulkan/buffers/stagedbuffer.hpp>
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/commandpools/commandpool.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/synchronization/queuesync.hpp>
#include <graphics/vulkan/textures/texture.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>

#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::Utilities;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::Graphics::Vulkan::Synchronization;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Textures {

struct NormalTexture : public Texture {

  NormalTexture(VulkanBackend vbe, QueueSync qs, Allocator allocator,
                Factories::Vulkan::BufferFactory bf, CommandPool cp,
                std::string path)
      : Texture(vbe, qs, allocator, bf, cp) {

    assert(path.length() != 0);

    // Load the image pixels
    int texWidth, texHeight, texChannels;

    stbi_uc *pixels = stbi_load(path.c_str(), &texWidth, &texHeight,
                                &texChannels, STBI_rgb_alpha);

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    assert(pixels != nullptr);

    auto buffer = bf.CreateStagingBuffer(imageSize, pixels,
                                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    // Create a buffer and free pixels
    stbi_image_free(pixels);
    auto buf = buffer->GetVulkanBuffer();

    VkFormat colorFormat = GetColorFormat();

    // Create, transition and copy the image
    CreateImage(texWidth, texHeight, colorFormat, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                _textureImage);

    TransitionImageLayout(_textureImage, VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    CopyBufferToImage(buf, _textureImage, static_cast<uint32_t>(texWidth),
                      static_cast<uint32_t>(texHeight));

    TransitionImageLayout(_textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // Create and set the image view
    auto imageView = ImageView(vbe, _textureImage, colorFormat);
    _imageView = imageView.Get();
  }
};
} // namespace Textures
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy