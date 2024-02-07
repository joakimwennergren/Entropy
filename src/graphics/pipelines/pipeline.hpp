#pragma once

#include <array>

#include <graphics/shaders/shader.hpp>
#include <graphics/renderpasses/renderpass.hpp>
#include <filesystem/filesystem.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/data/pushcontant.hpp>
#include <servicelocators/servicelocator.hpp>
#include <graphics/devices/logical_device.hpp>
#include <graphics/swapchains/swapchain.hpp>
#include <graphics/descriptorsets/descriptorset.hpp>
#include <graphics/descriptorsetlayouts/descriptorsetlayout.hpp>
#include <graphics/descriptorpools/descriptorpool.hpp>

#include "spdlog/spdlog.h"

using namespace Symbios::Filesystem;
using namespace Symbios::Graphics::Shaders;
using namespace Entropy::Graphics::RenderPasses;
using namespace Symbios::Filesystem;
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

                void Build(const std::string name, const std::string vertexShader, const std::string fragmentShader, std::vector<VkDescriptorSetLayout> dsLayout);

                inline VkPipeline GetPipeline() { return _pipeline; };
                inline VkPipelineLayout GetPipelineLayout() { return _pipelineLayout; };
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
