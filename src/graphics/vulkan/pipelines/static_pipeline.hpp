#pragma once

#include <graphics/vulkan/pipelines/pipeline.hpp>

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Pipelines {
class StaticPipeline : public Pipeline {
public:
  StaticPipeline(std::shared_ptr<RenderPass> renderPass,
                 VkPolygonMode polygonMode)
      : Pipeline(renderPass, polygonMode) {
    auto dsLayouts = Setup();
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = dsLayouts.size();
    pipelineLayoutInfo.pSetLayouts = dsLayouts.data();
    pipelineLayoutInfo.pPushConstantRanges = nullptr; //&push_constant;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    glm::vec2 depthBounds = {0.0, 1.0};
    Build("StaticPipeline", "static_vert.spv", "static_frag.spv", dsLayouts,
          VK_TRUE, depthBounds, pipelineLayoutInfo, polygonMode);
  }

  // StaticPipeline(std::shared_ptr<RenderPass> renderPass,
  //                std::shared_ptr<ServiceLocator> serviceLocator,
  //                std::vector<char> vert_shader, std::vector<char> frag_shader,
  //                VkPolygonMode polygonMode)
  //     : Pipeline(renderPass, serviceLocator, polygonMode) {
  //   auto dsLayouts = Setup();
  //   VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  //   pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  //   pipelineLayoutInfo.setLayoutCount = dsLayouts.size();
  //   pipelineLayoutInfo.pSetLayouts = dsLayouts.data();
  //   pipelineLayoutInfo.pPushConstantRanges = nullptr; //&push_constant;
  //   pipelineLayoutInfo.pushConstantRangeCount = 0;
  //   glm::vec2 depthBounds = {0.0, 1.0};
  //   Build("StaticPipeline", vert_shader, frag_shader, dsLayouts, VK_TRUE,
  //         depthBounds, pipelineLayoutInfo, polygonMode);
  // }

private:
  std::vector<VkDescriptorSetLayout> Setup() {
    std::vector<VkDescriptorSetLayout> dsLayouts(2);

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding uboDynLayoutBinding{};
    uboDynLayoutBinding.binding = 1;
    uboDynLayoutBinding.descriptorCount = 1;
    uboDynLayoutBinding.descriptorType =
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    uboDynLayoutBinding.pImmutableSamplers = nullptr;
    uboDynLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 2;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding textureLayoutBinding{};
    textureLayoutBinding.binding = 3;
    textureLayoutBinding.descriptorCount = 1;
    textureLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    textureLayoutBinding.pImmutableSamplers = nullptr;
    textureLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::vector<VkDescriptorSetLayoutBinding> bindings = {
        uboLayoutBinding, uboDynLayoutBinding, samplerLayoutBinding,
        textureLayoutBinding};

    auto descriptorSetLayout0 =
        std::make_shared<DescriptorsetLayout>(_logicalDevice, bindings);

    VkDescriptorSetLayoutBinding samplerLayoutBinding1{};
    samplerLayoutBinding1.binding = 1;
    samplerLayoutBinding1.descriptorCount = 1;
    samplerLayoutBinding1.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    samplerLayoutBinding1.pImmutableSamplers = nullptr;
    samplerLayoutBinding1.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding texturesLayoutBinding1{};
    texturesLayoutBinding1.binding = 2;
    texturesLayoutBinding1.descriptorCount = 1;
    texturesLayoutBinding1.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    texturesLayoutBinding1.pImmutableSamplers = nullptr;
    texturesLayoutBinding1.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::vector<VkDescriptorSetLayoutBinding> textureBinding = {
        texturesLayoutBinding1, samplerLayoutBinding1};

    auto descriptorSetLayout1 =
        std::make_shared<DescriptorsetLayout>(_logicalDevice, textureBinding);

    dsLayouts[0] = descriptorSetLayout0->Get();
    dsLayouts[1] = descriptorSetLayout1->Get();

    descriptorSets.push_back(std::make_shared<Descriptorset>(
        _logicalDevice, _descriptorPool, descriptorSetLayout0));
    descriptorSets.push_back(std::make_shared<Descriptorset>(
        _logicalDevice, _descriptorPool, descriptorSetLayout1));

    return dsLayouts;
  }
};
} // namespace Pipelines
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
