#pragma once

#include <vulkan/vulkan.hpp>
#include "context.hpp"
#include "commandbuffer.hpp"

namespace Symbios
{
    namespace Graphics
    {
        namespace RenderPasses
        {
            class RenderPass
            {
            public:
                RenderPass(Symbios::Core::Context *context);
                ~RenderPass();

                void Begin(Symbios::Graphics::CommandBuffers::CommandBuffer *commandBuffer, uint32_t imageIndex);
                void End(Symbios::Graphics::CommandBuffers::CommandBuffer *commandBuffer);

                void CreateFramebuffers();

                VkRenderPass GetRenderPass() { return this->renderPass; };
                std::vector<VkFramebuffer> swapChainFramebuffers;

            private:
                Symbios::Core::Context * _context;
                VkRenderPass renderPass;
            };
        }
    }
}
