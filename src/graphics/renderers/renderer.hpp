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
                Renderer(Context *context);
                void Render();

            private:
                Context *_context;
                RenderPass *_renderPass;
                std::unique_ptr<Pipeline> _pipeline;
                CommandBuffer *_commandBuffer;

                VkSemaphore imageAvailableSemaphore;
                VkSemaphore renderFinishedSemaphore;
                VkFence inFlightFence;
            };
        }
    }
}
