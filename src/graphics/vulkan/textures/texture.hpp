#ifndef ENTROPY_TEXTURE_H
#define ENTROPY_TEXTURE_H

#include <stb_image.h>
#include <spdlog/spdlog.h>
#include <string>

#include <graphics/vulkan/buffers/stagedbuffer.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/textures/base_texture.hpp>
#include <graphics/vulkan/utilities/helpers.hpp>

#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

namespace Entropy::Graphics::Vulkan::Textures {
    struct Texture : BaseTexture {
        /**
         * Constructor that creates and loads a texture from the given file path.
         * This function initializes a Vulkan texture, manages the image loading, buffer creation,
         * image transitions, and assigns an associated image view.
         *
         * @param path The file path of the texture image. Must be a valid and non-empty path.
         *
         * @return None. This is a constructor and does not have an explicit return value.
         */
        explicit Texture(const std::string &path) {
            assert(!path.empty());

            int texWidth, texHeight, texChannels;
            stbi_set_flip_vertically_on_load(true);

            // Load the image pixels
            stbi_uc *pixels = stbi_load(path.c_str(), &texWidth, &texHeight,
                                        &texChannels, STBI_rgb_alpha);

            const VkDeviceSize imageSize = texWidth * texHeight * 4;
            assert(pixels != nullptr);

            const auto buffer =
                    StagingBuffer(imageSize, pixels, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

            // Create a buffer and free pixels
            stbi_image_free(pixels);
            const auto buf = buffer.GetVulkanBuffer();

            // Get the color format being used
            const VkFormat colorFormat = GetColorFormat();

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

            imageView = std::make_shared<ImageView>(_textureImage, colorFormat);

            BindDescriptorSet();
        }

        void BindDescriptorSet() {
            VkPhysicalDeviceProperties properties{};
            vkGetPhysicalDeviceProperties(_physicalDevice->Get(), &properties);

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

            VK_CHECK(vkCreateSampler(_logicalDevice->Get(), &samplerInfo, nullptr,
                &textureSampler));

            //const std::vector<VkDescriptorBindingFlags> bindingFlags = {
            //    VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT
            //};

            //VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo = {};
            //bindingFlagsInfo.sType =
            //        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT;
            //bindingFlagsInfo.bindingCount = bindingFlags.size();
            //bindingFlagsInfo.pBindingFlags = bindingFlags.data();

            VkDescriptorSetLayoutBinding samplerLayoutBinding{};
            samplerLayoutBinding.binding = 2;
            samplerLayoutBinding.descriptorCount = 1;
            samplerLayoutBinding.descriptorType =
                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            samplerLayoutBinding.pImmutableSamplers = nullptr;
            samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

            const std::array<VkDescriptorSetLayoutBinding, 1> bindings = {
                samplerLayoutBinding
            };

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();
            //layoutInfo.pNext = &bindingFlagsInfo;

            VK_CHECK(vkCreateDescriptorSetLayout(_logicalDevice->Get(), &layoutInfo,
                nullptr, &_descriptorSetLayout));

            const std::vector layouts(1, _descriptorSetLayout);

            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = _descriptorPool->Get();
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = layouts.data();

            VK_CHECK(vkAllocateDescriptorSets(_logicalDevice->Get(), &allocInfo,
                &descriptorSet));

            std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = imageView->Get();
            imageInfo.sampler = textureSampler;

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = descriptorSet;
            descriptorWrites[0].dstBinding = 2;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType =
                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(_logicalDevice->Get(),
                                   descriptorWrites.size(),
                                   descriptorWrites.data(), 0, nullptr);
        }
    };
} // namespace Entropy::Graphics::Vulkan::Textures

#endif // ENTROPY_TEXTURE_H
