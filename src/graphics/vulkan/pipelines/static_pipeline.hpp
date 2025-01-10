#pragma once

#include <filesystem/filesystem.hpp>
#include <graphics/vulkan/pipelinecaches/pipelinecache.hpp>
#include <graphics/vulkan/pipelines/pipeline.hpp>

namespace Entropy::Graphics::Vulkan::Pipelines
{
    struct StaticPipeline : Pipeline
    {
        /**
         * Constructs a StaticPipeline object.
         *
         * @param renderPass A shared pointer to a RenderPass object that this pipeline will use.
         * @return A constructed StaticPipeline object.
         */
        explicit StaticPipeline(const std::shared_ptr<RenderPass> &renderPass)
            : Pipeline(renderPass)
        {
            const auto dsLayouts = CreateDescriptorSets();
            _shader = std::make_shared<Shader>(GetShadersDir() + "static/static_vert.spv",
                                               GetShadersDir() + "static/static_frag.spv");
            Build(_shader, dsLayouts);
        }

    private:
        /**
         * Creates descriptor set layouts for the static pipeline, setting up
         * bindings for instance buffers, uniform buffers, and combined image samplers.
         *
         * @return A vector of Vulkan descriptor set layouts.
         */
        std::vector<VkDescriptorSetLayout> CreateDescriptorSets()
        {
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

            const std::vector bindings = {
                instanceLayoutBinding,
                uboLayoutBinding};

            const std::vector bindings2 = {
                samplerLayoutBinding};

            const std::vector<VkDescriptorBindingFlags> bindingFlags0 = {};

            const std::vector<VkDescriptorBindingFlags> bindingFlags1 = {
                // VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT
            };

            const auto descriptorSetLayout0 = std::make_shared<DescriptorSetLayout>(bindings, bindingFlags0);
            const auto descriptorSetLayout1 = std::make_shared<DescriptorSetLayout>(bindings2, bindingFlags1);

            dsLayouts[0] = descriptorSetLayout0->Get();
            dsLayouts[1] = descriptorSetLayout1->Get();

            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = _descriptorPool->Get();
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = dsLayouts.data();

            VK_CHECK(vkAllocateDescriptorSets(_logicalDevice->Get(), &allocInfo,
                                              &descriptorSet));

            return dsLayouts;
        }
    };
} // namespace Entropy::Graphics::Vulkan::Pipelines
