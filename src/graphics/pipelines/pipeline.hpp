#pragma once

#include <array>

#include <graphics/shaders/shader.hpp>
#include <graphics/renderpasses/renderpass.hpp>
#include <filesystem/filesystem.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/data/pushcontant.hpp>
#include <global/vulkancontext.hpp>

using namespace Symbios::Filesystem;
using namespace Symbios::Graphics::Shaders;
using namespace Entropy::Graphics::RenderPasses;
using namespace Symbios::Filesystem;
using namespace Entropy::Global;

namespace Entropy
{
    namespace Graphics
    {
        namespace Pipelines
        {
            class Pipeline
            {
            public:
                Pipeline(std::shared_ptr<RenderPass> renderPass);
                Pipeline(std::shared_ptr<RenderPass> renderPass, uint32_t *vertContent, uint32_t vertSize, uint32_t *fragContent, uint32_t fragSize);
                ~Pipeline();

                inline VkPipeline GetPipeline() { return this->_pipeline; };
                inline VkPipelineLayout GetPipelineLayout() { return this->_pipelineLayout; };

            private:
                std::array<VkPipelineShaderStageCreateInfo, 2> CreateShaderStages();
                VkPipelineDynamicStateCreateInfo CreateDynamicState();
                VkPipelineInputAssemblyStateCreateInfo CreateInputAssembly();
                VkPipelineViewportStateCreateInfo CreateViewportState();
                VkPipelineRasterizationStateCreateInfo CreateRasterizer();
                VkPipelineMultisampleStateCreateInfo CreateMultisampling();
                VkPipelineColorBlendStateCreateInfo CreateColorBlendning();
                std::vector<VkDescriptorSetLayout> CreateDescriptorSetLayouts();
                std::vector<VkPipelineVertexInputStateCreateInfo> CreateVertexInputStates();
                VkPushConstantRange CreatePushContantRange();
                VkPipelineLayout CreatePipelineLayout(std::vector<VkDescriptorSetLayout> dsLayouts, VkPushConstantRange push_constant);

                std::unique_ptr<Shader> _shader;
                std::shared_ptr<RenderPass> _renderPass;

                VkPipelineLayout _pipelineLayout;
                VkPipeline _pipeline;
                VkDescriptorSetLayout _descriptorSetLayout;

                VulkanContext *_vkContext;
            };
        }
    }
}
