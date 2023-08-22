#pragma once

#include "context.hpp"
#include "renderpass.hpp"
#include "pipeline.hpp"
#include "commandbuffer.hpp"

using namespace Symbios::Core;
using namespace Symbios::Graphics;

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
                ~Renderer();

                void Render();

            private:
                Context *_context;
                Symbios::Graphics::RenderPasses::RenderPass *_renderPass;
                Symbios::Graphics::Pipelines::Pipeline *_pipeline;
                Symbios::Graphics::CommandBuffers::CommandBuffer *_commandBuffer;

                VkSemaphore imageAvailableSemaphore;
                VkSemaphore renderFinishedSemaphore;
                VkFence inFlightFence;
            };
        }
    }
}
