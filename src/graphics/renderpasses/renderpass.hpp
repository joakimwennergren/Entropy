#pragma once

#include <vulkan/vulkan.hpp>
#include "context.hpp"
#include "commandbuffer.hpp"

using namespace Symbios::Core;
using namespace Symbios::Graphics::CommandBuffers;

namespace Symbios
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
                 */
                RenderPass() = default;

                /**
                 * @brief Construct a new Render Pass object
                 *
                 * @param context
                 */
                RenderPass(std::shared_ptr<Context> context);

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
                 * @brief Get the Render Pass object
                 *
                 * @return VkRenderPass
                 */
                VkRenderPass GetRenderPass() { return this->_renderPass; };

            private:
                void CreateFramebuffers();
                std::shared_ptr<Context> _context;
                VkRenderPass _renderPass;
                std::vector<VkFramebuffer> _swapChainFramebuffers;
            };
        }
    }
}
