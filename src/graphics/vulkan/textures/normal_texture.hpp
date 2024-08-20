#pragma once

#include "vulkan/vulkan_core.h"
#include <ft2build.h>
#include <stb_image.h>
#include FT_FREETYPE_H

#include <spdlog/spdlog.h>
#include <string>

#include "tiny_gltf.h"
#include <config.hpp>

#include <graphics/vulkan/buffers/buffer.hpp>
#include <graphics/vulkan/buffers/stagedbuffer.hpp>
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/commandpools/commandpool.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/synchronization/queuesync.hpp>
#include <graphics/vulkan/textures/texture.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>

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

namespace Entropy
{
  namespace Graphics
  {
    namespace Vulkan
    {
      namespace Textures
      {

        struct NormalTexture : public Texture
        {

          VkSampler _textureSampler;
          VkDescriptorSetLayout _descriptorSetLayout;

          NormalTexture(std::string path) : Texture()
          {
            assert(path.length() != 0);

            // Load the image pixels
            int texWidth, texHeight, texChannels;

            stbi_set_flip_vertically_on_load(true);

            stbi_uc *pixels = stbi_load(path.c_str(), &texWidth, &texHeight,
                                        &texChannels, STBI_rgb_alpha);

            VkDeviceSize imageSize = texWidth * texHeight * 4;

            assert(pixels != nullptr);

            auto buffer = StagedBuffer(imageSize, pixels,
                                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            // Create a buffer and free pixels
            stbi_image_free(pixels);
            auto buf = buffer.GetVulkanBuffer();

            VkFormat colorFormat = GetColorFormat();

            // // Create, transition and copy the image
            CreateImage(texWidth, texHeight, colorFormat, VK_IMAGE_TILING_OPTIMAL,
                        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                        _textureImage);

            TransitionImageLayout(_textureImage, VK_IMAGE_LAYOUT_UNDEFINED,
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

            CopyBufferToImage(buf, _textureImage, static_cast<uint32_t>(texWidth),
                              static_cast<uint32_t>(texHeight));

            TransitionImageLayout(_textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

            // // Create and set the image view
            auto imageView = ImageView(_textureImage, colorFormat);
            _imageView = imageView.Get();

            // // Get required depenencies

            VkPhysicalDeviceProperties properties{};
            vkGetPhysicalDeviceProperties(_physicalDevice->Get(),
                                          &properties);

            VkSamplerCreateInfo samplerInfo{};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.magFilter = VK_FILTER_LINEAR;
            samplerInfo.minFilter = VK_FILTER_LINEAR;

            samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.anisotropyEnable = VK_TRUE;
            samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
            samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
            samplerInfo.unnormalizedCoordinates = VK_FALSE;
            samplerInfo.compareEnable = VK_FALSE;
            samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            samplerInfo.mipLodBias = 0.0f;
            samplerInfo.minLod = 0.0f;
            samplerInfo.maxLod = 0.0f;

            if (vkCreateSampler(_logicalDevice->Get(), &samplerInfo,
                                nullptr, &_textureSampler) != VK_SUCCESS)
            {
              throw std::runtime_error("failed to create texture sampler!");
            }

            VkDescriptorSetLayoutBinding samplerLayoutBinding{};
            samplerLayoutBinding.binding = 2;
            samplerLayoutBinding.descriptorCount = 1;
            samplerLayoutBinding.descriptorType =
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            samplerLayoutBinding.pImmutableSamplers = nullptr;
            samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

            std::array<VkDescriptorSetLayoutBinding, 1> bindings = {
                samplerLayoutBinding};
            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            if (vkCreateDescriptorSetLayout(_logicalDevice->Get(),
                                            &layoutInfo, nullptr,
                                            &_descriptorSetLayout) != VK_SUCCESS)
            {
              throw std::runtime_error("failed to create descriptor set layout!");
            }

            std::vector<VkDescriptorSetLayout> layouts(1, _descriptorSetLayout);

            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = _descriptorPool->Get();
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = layouts.data();

            if (vkAllocateDescriptorSets(_logicalDevice->Get(), &allocInfo,
                                         &_descriptorSet) != VK_SUCCESS)
            {
              throw std::runtime_error("failed to allocate descriptor sets!");
            }

            std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = _imageView;
            imageInfo.sampler = _textureSampler;

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = _descriptorSet;
            descriptorWrites[0].dstBinding = 2;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType =
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(_logicalDevice->Get(),
                                   static_cast<uint32_t>(descriptorWrites.size()),
                                   descriptorWrites.data(), 0, nullptr);
          }
        };
      } // namespace Textures
    } // namespace Vulkan
  } // namespace Graphics
} // namespace Entropy
