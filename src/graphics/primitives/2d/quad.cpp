#include "quad.hpp"

using namespace Entropy::Graphics::Primitives;

Quad::Quad() {


  // std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

  // std::vector<Vertex> vertices = {
  //     {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
  //     {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
  //     {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
  //     {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

  // // vertexBuffer = std::make_unique<VertexBuffer>(vertices);
  // //  indexBuffer = std::make_unique<IndexBuffer<uint16_t>>(indices);

  // _blank = std::make_unique<Texture>(serviceLocator);

  // _blank->CreateTextureImage(Filesystem::GetTexturesDir() + "blank.png");

  // UpdateDescriptorSets();
}

Quad::~Quad() {}

void Quad::UpdateDescriptorSets() {
  /*
  // Get required depenencies
  auto logicalDevice = _serviceLocator->GetService<LogicalDevice>();
  auto physicalDevice = _serviceLocator->GetService<PhysicalDevice>();
  auto descriptorPool = _serviceLocator->GetService<DescriptorPool>();

  VkPhysicalDeviceProperties properties{};
  vkGetPhysicalDeviceProperties(physicalDevice->Get(), &properties);

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

  if (vkCreateSampler(logicalDevice->Get(), &samplerInfo, nullptr,
                      &_textureSampler) != VK_SUCCESS) {
    throw std::runtime_error("failed to create texture sampler!");
  }

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

  if (vkCreateDescriptorSetLayout(logicalDevice->Get(), &layoutInfo, nullptr,
                                  &_descriptorSetLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor set layout!");
  }

  std::vector<VkDescriptorSetLayout> layouts(1, _descriptorSetLayout);

  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = descriptorPool->Get();
  allocInfo.descriptorSetCount = 1; // MAX_CONCURRENT_FRAMES_IN_FLIGHT;
  allocInfo.pSetLayouts = layouts.data();

  if (vkAllocateDescriptorSets(logicalDevice->Get(), &allocInfo,
                               &_descriptorSet) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate descriptor sets!");
  }

  VkDescriptorImageInfo imageInfo{};
  imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  imageInfo.imageView = _blank->GetImageView();
  imageInfo.sampler = _textureSampler;

  std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

  descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[0].dstSet = _descriptorSet;
  descriptorWrites[0].dstBinding = 2;
  descriptorWrites[0].dstArrayElement = 0;
  descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
  descriptorWrites[0].descriptorCount = 1;
  descriptorWrites[0].pImageInfo = &imageInfo;

  descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[1].dstSet = _descriptorSet;
  descriptorWrites[1].dstBinding = 1;
  descriptorWrites[1].dstArrayElement = 0;
  descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
  descriptorWrites[1].descriptorCount = 1;
  descriptorWrites[1].pImageInfo = &imageInfo;

  vkUpdateDescriptorSets(logicalDevice->Get(),
                         static_cast<uint32_t>(descriptorWrites.size()),
                         descriptorWrites.data(), 0, nullptr);

                        */
}
