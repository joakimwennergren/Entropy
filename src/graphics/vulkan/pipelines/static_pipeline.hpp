#pragma once

#include "filesystem/filesystem.hpp"
#include "graphics/vulkan/pipelinecaches/pipelinecache.hpp"
#include "vulkan/vulkan_core.h"
#include <graphics/vulkan/pipelines/pipeline.hpp>

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Pipelines {

struct StaticPipeline : public Pipeline {

  StaticPipeline(std::shared_ptr<RenderPass> renderPass)
      : Pipeline(renderPass) {
    auto dsLayouts = CreateDescriptorSets();
    _shader = std::make_shared<Shader>(GetShadersDir() + "static_vert.spv",
                                       GetShadersDir() + "static_frag.spv");
    Build(_shader, dsLayouts);
  }

private:
  std::vector<VkDescriptorSetLayout> CreateDescriptorSets() {

    std::vector<VkDescriptorSetLayout> dsLayouts(2);

    VkDescriptorSetLayoutBinding instanceLayoutBinding = {};
    instanceLayoutBinding.binding = 0; // Binding for instance buffer
    instanceLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    instanceLayoutBinding.descriptorCount = 1;
    instanceLayoutBinding.stageFlags = VK_SHADER_STAGE_ALL;
    instanceLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 1;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_ALL;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 2;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType =
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::vector<VkDescriptorSetLayoutBinding> bindings = {instanceLayoutBinding,
                                                          uboLayoutBinding};

    std::vector<VkDescriptorSetLayoutBinding> bindings2 = {
        samplerLayoutBinding};

    std::vector<VkDescriptorBindingFlagsEXT> bindingFlags0 = {};

    std::vector<VkDescriptorBindingFlagsEXT> bindingFlags1 = {
        // VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT_EXT, // For binding 0
    };

    auto descriptorSetLayout0 = DescriptorsetLayout(bindings, bindingFlags0);

    auto descriptorSetLayout1 = DescriptorsetLayout(bindings2, bindingFlags1);

    dsLayouts[0] = descriptorSetLayout0.Get();
    dsLayouts[1] = descriptorSetLayout1.Get();

    descriptorSets.push_back(Descriptorset(descriptorSetLayout0));
    descriptorSets.push_back(Descriptorset(descriptorSetLayout1));

    return dsLayouts;
  }
};
} // namespace Pipelines
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
