#include "default_renderer.hpp"

using namespace Symbios::Graphics::Renderers;

Default::Default(Context *context, Symbios::Graphics::CommandBuffers::CommandBuffer commandBuffer)
{
    this->_context = context;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(_context->GetSwapChainExtent().width);
    viewport.height = static_cast<float>(_context->GetSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer.GetCommandBuffer(), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = _context->GetSwapChainExtent();
    vkCmdSetScissor(commandBuffer.GetCommandBuffer(), 0, 1, &scissor);

    vkCmdDraw(commandBuffer.GetCommandBuffer(), 3, 1, 0, 0);
}
