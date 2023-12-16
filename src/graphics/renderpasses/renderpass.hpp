#pragma once

#include <vulkan/vulkan.hpp>
#include <global/vulkancontext.hpp>
#include <graphics/commandbuffers/commandbuffer.hpp>
#include <servicelocators/servicelocator.hpp>
#include <graphics/swapchains/swapchain.hpp>
#include <graphics/devices/logical_device.hpp>

#include "spdlog/spdlog.h"

using namespace Entropy::Graphics::CommandBuffers;
using namespace Entropy::Global;
using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Swapchains;
using namespace Entropy::Graphics::Devices;

namespace Entropy
{
    namespace Graphics
    {
        namespace RenderPasses
        {
            class RenderPass
            {
            public:
                /**
                 * @brief Construct a new Render Pass object
                 *
                 * @param context
                 */
                RenderPass(std::shared_ptr<ServiceLocator> serviceLocator);

                /**
                 * @brief Destroy the Render Pass object
                 *
                 */
                ~RenderPass();

                /**
                 * @brief
                 *
                 * @param commandBuffer
                 * @param imageIndex
                 */
                void Begin(std::shared_ptr<CommandBuffer> commandBuffer, uint32_t imageIndex);

                /**
                 * @brief
                 *
                 * @param commandBuffer
                 */
                void End(std::shared_ptr<CommandBuffer> commandBuffer);

                /**
                 * @brief
                 *
                 */
                void RecreateFrameBuffers();

                /**
                 * @brief Get the Render Pass object
                 *
                 * @return VkRenderPass
                 */
                VkRenderPass GetRenderPass() { return this->_renderPass; };

            private:
                void CreateFramebuffers();
                VkRenderPass _renderPass;
                std::vector<VkFramebuffer> _swapChainFramebuffers;

                std::shared_ptr<Swapchain> _swapChain;
                std::shared_ptr<LogicalDevice> _logicalDevice;
            };
        }
    }
}
