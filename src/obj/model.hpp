#pragma once

#include "graphics/vulkan/textures/normal_texture.hpp"
#include <tiny_obj_loader.h>

#include <data/vertex.hpp>
#include <graphics/vulkan/buffers/indexbuffer.hpp>
#include <graphics/vulkan/buffers/vertexbuffer.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/textures/texture.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Data;

namespace Entropy
{
  namespace OBJ
  {
    class ObjModel
    {
    public:
      ObjModel() {}

      void loadFromFile(std::string file, std::string tex)
      {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                              file.c_str()))
        {
          throw std::runtime_error(warn + err);
        }

        for (const auto &shape : shapes)
        {
          for (const auto &index : shape.mesh.indices)
          {
            Vertex vertex{};

            vertex.position = {attrib.vertices[3 * index.vertex_index + 0],
                               attrib.vertices[3 * index.vertex_index + 1],
                               attrib.vertices[3 * index.vertex_index + 2]};

            vertex.uv0 = {attrib.texcoords[2 * index.texcoord_index + 0],
                          1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

            vertex.color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};

            vertices.push_back(vertex);
          }
        }

        // vertexBuffer = _bufferFactory.CreateVertexBuffer(vertices);

        // texture = _textureFactory.CreateNormalTexture(tex);

        /*

        texture = new Texture();
        texture->CreateTextureImage(tex);

        VkPhysicalDeviceProperties properties{};
        // vkGetPhysicalDeviceProperties(
        //     _serviceLocator->GetService<PhysicalDevice>()->Get(), &properties);

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

        VkSampler _textureSampler;

        // if (vkCreateSampler(_serviceLocator->GetService<LogicalDevice>()->Get(),
        //                     &samplerInfo, nullptr,
        //                     &_textureSampler) != VK_SUCCESS) {
        //   throw std::runtime_error("failed to create texture sampler!");
        // }

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutBinding texturesLayoutBinding{};
        texturesLayoutBinding.binding = 2;
        texturesLayoutBinding.descriptorCount = 1;
        texturesLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        texturesLayoutBinding.pImmutableSamplers = nullptr;
        texturesLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = {
            samplerLayoutBinding, texturesLayoutBinding};
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        VkDescriptorSetLayout _descriptorSetLayout;

        // if (vkCreateDescriptorSetLayout(
        //         _serviceLocator->GetService<LogicalDevice>()->Get(), &layoutInfo,
        //         nullptr, &_descriptorSetLayout) != VK_SUCCESS) {
        //   throw std::runtime_error("failed to create descriptor set layout!");
        // }

        std::vector<VkDescriptorSetLayout> layouts(1, _descriptorSetLayout);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        // allocInfo.descriptorPool =
        //     _serviceLocator->GetService<DescriptorPool>()->Get();
        allocInfo.descriptorSetCount = 1; // MAX_CONCURRENT_FRAMES_IN_FLIGHT;
        allocInfo.pSetLayouts = layouts.data();

        // if (vkAllocateDescriptorSets(
        //         _serviceLocator->GetService<LogicalDevice>()->Get(), &allocInfo,
        //         &ds) != VK_SUCCESS) {
        //   throw std::runtime_error("failed to allocate descriptor sets!");
        // }

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = texture->GetImageView();
        imageInfo.sampler = _textureSampler;

        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = ds;
        descriptorWrites[0].dstBinding = 2;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo = &imageInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = ds;
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        //
        vkUpdateDescriptorSets(_serviceLocator->GetService<LogicalDevice>()->Get(),
        //                        static_cast<uint32_t>(descriptorWrites.size()),
        //                        descriptorWrites.data(), 0, nullptr);
        */
      }

      std::vector<Vertex> vertices;
      std::shared_ptr<VertexBuffer> vertexBuffer;
      std::shared_ptr<NormalTexture> texture;

    private:
    };
  } // namespace OBJ
} // namespace Entropy