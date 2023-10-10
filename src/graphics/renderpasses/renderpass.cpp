#include "renderpass.hpp"

using namespace Symbios::Graphics::RenderPasses;

RenderPass::RenderPass()
{
    // Store vulkan ctx
    _context = Global::VulkanContext::GetInstance()->GetVulkanContext();

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = _context->GetSwapChainImageFormat();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;

    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;

    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(_context->GetLogicalDevice(), &renderPassInfo, nullptr, &this->_renderPass) != VK_SUCCESS)
    {
        PLOG_ERROR << "failed to create render pass!";
        exit(EXIT_FAILURE);
    }

    this->CreateFramebuffers();
}

RenderPass::~RenderPass()
{
    for (auto framebuffer : this->_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(_context->GetLogicalDevice(), framebuffer, nullptr);
    }

    vkDestroyRenderPass(_context->GetLogicalDevice(), this->_renderPass, nullptr);
}

void RenderPass::Begin(std::shared_ptr<CommandBuffer> commandBuffer, uint32_t imageIndex)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = this->_renderPass;
    renderPassInfo.framebuffer = this->_swapChainFramebuffers[imageIndex];

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = _context->GetSwapChainExtent();

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer->GetCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void RenderPass::End(std::shared_ptr<CommandBuffer> commandBuffer)
{
    vkCmdEndRenderPass(commandBuffer->GetCommandBuffer());
}

void RenderPass::CreateFramebuffers()
{
    this->_swapChainFramebuffers.resize(_context->GetSwapChainImageViews().size());

    for (size_t i = 0; i < _context->GetSwapChainImageViews().size(); i++)
    {
        VkImageView attachments[] = {
            _context->GetSwapChainImageViews()[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = this->_renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = _context->GetSwapChainExtent().width;
        framebufferInfo.height = _context->GetSwapChainExtent().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(_context->GetLogicalDevice(), &framebufferInfo, nullptr, &this->_swapChainFramebuffers[i]) != VK_SUCCESS)
        {
            PLOG_ERROR << "Failed to create framebuffer!";
            exit(EXIT_FAILURE);
        }
    }
}

void RenderPass::RecreateFrameBuffers()
{
    for (auto framebuffer : this->_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(_context->GetLogicalDevice(), framebuffer, nullptr);
    }

    this->CreateFramebuffers();
}
