#pragma once

#include <graphics/pipelines/pipeline.hpp>

namespace Entropy
{
    namespace Graphics
    {
        namespace Pipelines
        {
            class SkinnedPipeline : public Pipeline
            {
            public:
                SkinnedPipeline(std::shared_ptr<RenderPass> renderPass, std::shared_ptr<ServiceLocator> serviceLocator) : Pipeline(renderPass, serviceLocator)
                {
                    std::vector<VkDescriptorSetLayout> dsLayouts(3);

                    VkDescriptorSetLayout tempLayout;

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

                    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {texturesLayoutBinding, samplerLayoutBinding};
                    VkDescriptorSetLayoutCreateInfo layoutInfo{};
                    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
                    layoutInfo.pBindings = bindings.data();

                    if (vkCreateDescriptorSetLayout(_logicalDevice->Get(), &layoutInfo, nullptr, &tempLayout) != VK_SUCCESS)
                    {
                        spdlog::error("Failed to create descriptor set layout!");
                        return;
                    }

                    VkDescriptorSetLayout tempLayout2;
                    VkDescriptorSetLayoutBinding ssboBinding{};
                    ssboBinding.binding = 0;
                    ssboBinding.descriptorCount = 1;
                    ssboBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    ssboBinding.pImmutableSamplers = nullptr;
                    ssboBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

                    std::array<VkDescriptorSetLayoutBinding, 1> bindings2 = {ssboBinding};
                    VkDescriptorSetLayoutCreateInfo layoutInfo2{};
                    layoutInfo2.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                    layoutInfo2.bindingCount = static_cast<uint32_t>(bindings2.size());
                    layoutInfo2.pBindings = bindings2.data();

                    if (vkCreateDescriptorSetLayout(_logicalDevice->Get(), &layoutInfo2, nullptr, &tempLayout2) != VK_SUCCESS)
                    {
                        spdlog::error("Failed to create descriptor set layout!");
                        return;
                    }

                    dsLayouts[0] = _descriptorSetLayout->Get();
                    dsLayouts[1] = tempLayout;
                    dsLayouts[2] = tempLayout2;

                    Build("SkinnedPipeline", "vert.spv", "frag.spv", dsLayouts);
                }
            };
        }
    }
}
