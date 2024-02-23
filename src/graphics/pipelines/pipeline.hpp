#pragma once

#include <array>
#include <utility>

#include <graphics/shaders/shader.hpp>
#include <graphics/renderpasses/renderpass.hpp>
#include <filesystem/filesystem.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/data/pushcontants.hpp>
#include <servicelocators/servicelocator.hpp>
#include <graphics/devices/logical_device.hpp>
#include <graphics/swapchains/swapchain.hpp>
#include <graphics/descriptorsets/descriptorset.hpp>
#include <graphics/descriptorsetlayouts/descriptorsetlayout.hpp>
#include <graphics/descriptorpools/descriptorpool.hpp>

#include "spdlog/spdlog.h"

using namespace Entropy::Filesystem;
using namespace Symbios::Graphics::Shaders;
using namespace Entropy::Graphics::RenderPasses;
using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Swapchains;
using namespace Entropy::Graphics::Descriptorsets;
using namespace Entropy::Graphics::DescriptorPools;
using namespace Entropy::Graphics::Devices;

namespace Entropy
{
    namespace Graphics
    {
        namespace Pipelines
        {
            class Pipeline
            {
            public:
                Pipeline(std::shared_ptr<RenderPass> renderPass, std::shared_ptr<ServiceLocator> serviceLocator);
                ~Pipeline();

                void Setup(std::unique_ptr<Shader> shader, std::vector<VkDescriptorSetLayout> dsLayout, bool depthWrite, VkPipelineLayoutCreateInfo pipelinelayout);

                void Build(const std::string name, const std::string vertexShader, const std::string fragmentShader, std::vector<VkDescriptorSetLayout> dsLayout, bool depthWrite, VkPipelineLayoutCreateInfo pipelinelayout);
                void Build(const std::string name, std::vector<char> vert_shader, std::vector<char> frag_shader, std::vector<VkDescriptorSetLayout> dsLayout, bool depthWrite, VkPipelineLayoutCreateInfo pipelinelayout);
                inline VkPipeline GetPipeline()
                {
                    return _pipeline;
                };
                inline VkPipelineLayout GetPipelineLayout()
                {
                    return _pipelineLayout;
                };
                std::vector<std::shared_ptr<Descriptorset>> descriptorSets;

            protected:
                VkPipelineLayout _pipelineLayout;
                VkPipeline _pipeline;
                std::shared_ptr<RenderPass> _renderPass;
                std::shared_ptr<LogicalDevice> _logicalDevice;
                std::shared_ptr<Swapchain> _swapchain;
                std::shared_ptr<DescriptorsetLayout> _descriptorSetLayout;
                std::shared_ptr<DescriptorPool> _descriptorPool;
                std::shared_ptr<ServiceLocator> _serviceLocator;
            };
        }
    }
}
