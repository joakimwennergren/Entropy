#pragma once

#include "context.hpp"
#include "renderpass.hpp"
#include "pipeline.hpp"
#include "commandbuffer.hpp"

using namespace Symbios::Core;
using namespace Symbios::Graphics::Pipelines;
using namespace Symbios::Graphics::RenderPasses;
using namespace Symbios::Graphics::CommandBuffers;

namespace Symbios
{
    namespace Graphics
    {
        namespace Renderers
        {
            class Renderer
            {
            public:
                /**
                 * @brief Construct a new Renderer object
                 *
                 */
                Renderer() = default;
                /**
                 * @brief Construct a new Renderer object
                 *
                 * @param context
                 */
                Renderer(std::shared_ptr<Context> context);

                /**
                 * @brief
                 *
                 */
                void Render();

                /**
                 * @brief
                 *
                 */
                ~Renderer();

            private:
                std::shared_ptr<Context> _context;
                std::shared_ptr<RenderPass> _renderPass;
                std::shared_ptr<CommandBuffer> _commandBuffer;

                std::unique_ptr<Pipeline> _pipeline;

                VkSemaphore imageAvailableSemaphore;
                VkSemaphore renderFinishedSemaphore;
                VkFence inFlightFence;
            };
        }
    }
}
