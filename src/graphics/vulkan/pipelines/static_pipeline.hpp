#pragma once

#include "filesystem/filesystem.hpp"
#include <graphics/vulkan/pipelines/pipeline.hpp>

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Pipelines {

struct StaticPipeline : public Pipeline {

  StaticPipeline(VulkanBackend vbe, RenderPass rp, Swapchain sc,
                 DescriptorPool dp, DescriptorSetLayoutFactory dslf,
                 DescriptorSetFactory dsf)
      : Pipeline(vbe, rp, sc, dp, dslf, dsf) {
    auto dsLayouts = CreateDescriptorSets();
    Shader shader = Shader(vbe, GetShadersDir() + "static_vert.spv",
                           GetShadersDir() + "static_frag.spv");
    Build(shader, dsLayouts);
  }

private:
  std::vector<VkDescriptorSetLayout> CreateDescriptorSets() {

    std::vector<VkDescriptorSetLayout> dsLayouts(2);

    // VkDescriptorSetLayoutBinding uboLayoutBinding{};
    // uboLayoutBinding.binding = 0;
    // uboLayoutBinding.descriptorCount = 1;
    // uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    // uboLayoutBinding.pImmutableSamplers = nullptr;
    // uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

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
        uboDynLayoutBinding, samplerLayoutBinding, textureLayoutBinding};

    auto descriptorSetLayout0 =
        _descriptorSetLayoutFactory.CreateLayout(bindings);

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
        _descriptorSetLayoutFactory.CreateLayout(textureBinding);

    dsLayouts[0] = descriptorSetLayout0.Get();
    dsLayouts[1] = descriptorSetLayout1.Get();

    descriptorSets.push_back(
        _descriptorSetFactory.CreateDescriptorSet(descriptorSetLayout0));
    descriptorSets.push_back(
        _descriptorSetFactory.CreateDescriptorSet(descriptorSetLayout1));

    return dsLayouts;
  }
};
} // namespace Pipelines
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
