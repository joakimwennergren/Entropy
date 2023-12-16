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
#include <graphics/descriptorsetlayouts/descriptorsetlayout.hpp>

#include "spdlog/spdlog.h"

using namespace Symbios::Filesystem;
using namespace Symbios::Graphics::Shaders;
using namespace Entropy::Graphics::RenderPasses;
using namespace Symbios::Filesystem;
using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Swapchains;
using namespace Entropy::Graphics::DescriptorsetLayouts;
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
                Pipeline(std::shared_ptr<RenderPass> renderPass, uint32_t *vertContent, uint32_t vertSize, uint32_t *fragContent, uint32_t fragSize);
                ~Pipeline();

                inline VkPipeline GetPipeline() { return this->_pipeline; };
                inline VkPipelineLayout GetPipelineLayout() { return this->_pipelineLayout; };

            private:
                VkPipelineLayout _pipelineLayout;
                VkPipeline _pipeline;
                VkDescriptorSetLayout _descriptorSetLayout;
                std::shared_ptr<RenderPass> _renderPass;
                std::shared_ptr<LogicalDevice> _logicalDevice;
                std::shared_ptr<Swapchain> _swapchain;
                std::shared_ptr<DescriptorsetLayout> _descriptorsetLayout;
                std::shared_ptr<ServiceLocator> _serviceLocator;
            };
        }
    }
}
