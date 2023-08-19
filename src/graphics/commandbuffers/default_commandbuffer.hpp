#pragma once

#include "context.hpp"
#include "renderpass.hpp"

namespace Symbios
{
    namespace Graphics
    {
        namespace CommandBuffers
        {
            class Default
            {
            public:
                Default(Symbios::Core::Context *context);
                ~Default();

                void Record(uint32_t imageIndex, Symbios::Graphics::RenderPasses::Default *renderPass);
                VkCommandBuffer GetCommandBuffer() { return this->commandBuffer; };

            private:
                Symbios::Core::Context *_context;
                VkCommandPool commandPool;
                VkCommandBuffer commandBuffer;
            };
        }
    }
}
