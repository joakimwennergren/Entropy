#pragma once

#include <graphics/data/pushcontants.hpp>
#include <graphics/vulkan/pipelines/pipeline.hpp>

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Pipelines {
class GUIPipeline : public Pipeline {
public:
  GUIPipeline(VulkanBackend vbe, RenderPass rp, Swapchain sc, DescriptorPool dp, DescriptorSetLayoutFactory dslf)
      : Pipeline(vbe, rp, sc, dp, dslf) {
    auto dsLayouts = Setup();
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    pushConstantRange.size = sizeof(PushConstBlock);
    pushConstantRange.offset = 0;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = dsLayouts.size();
    pipelineLayoutInfo.pSetLayouts = dsLayouts.data();
    pipelineLayoutInfo.pPushConstantRanges =
        &pushConstantRange; //&push_constant;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    glm::vec2 depthBounds = {0.0, 1.0};
    Build("GUIPipeline", "ui_vert.spv", "ui_frag.spv", dsLayouts, VK_FALSE,
          depthBounds, pipelineLayoutInfo, VK_POLYGON_MODE_FILL);
  }

private:
  std::vector<VkDescriptorSetLayout> Setup() {
    std::vector<VkDescriptorSetLayout> dsLayouts(1);

    VkDescriptorSetLayoutBinding samplerLayoutBinding1{};
    samplerLayoutBinding1.binding = 0;
    samplerLayoutBinding1.descriptorCount = 1;
    samplerLayoutBinding1.descriptorType =
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding1.pImmutableSamplers = nullptr;
    samplerLayoutBinding1.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::vector<VkDescriptorSetLayoutBinding> textureBinding = {
        samplerLayoutBinding1};

    auto descriptorSetLayout0 =
        std::make_shared<DescriptorsetLayout>(_logicalDevice, textureBinding);

    dsLayouts[0] = descriptorSetLayout0->Get();

    descriptorSets.push_back(std::make_shared<Descriptorset>(
        _logicalDevice, _descriptorPool, descriptorSetLayout0));

    return dsLayouts;
  }
};
} // namespace Pipelines
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
