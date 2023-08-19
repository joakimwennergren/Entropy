#pragma once

#include <vulkan/vulkan.hpp>
#include "context.hpp"

namespace Symbios
{
    namespace Graphics
    {
        namespace RenderPasses
        {
            class Default
            {
            public:
                Default(Symbios::Core::Context *context);
                ~Default();
                void CreateFramebuffers();

                VkRenderPass GetRenderPass() { return this->renderPass; };
                std::vector<VkFramebuffer> swapChainFramebuffers;

            private:
                Symbios::Core::Context * _context;
                VkRenderPass renderPass;
                // FrameBuffers
            };
        }

    }

}
