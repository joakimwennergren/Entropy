#include "renderpass.hpp"

using namespace Entropy::Graphics::RenderPasses;

RenderPass::RenderPass()
{
    VulkanContext *vkContext = VulkanContext::GetInstance();

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = vkContext->swapChainImageFormat;
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

    if (vkCreateRenderPass(vkContext->logicalDevice, &renderPassInfo, nullptr, &this->_renderPass) != VK_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }

    this->CreateFramebuffers();
}

RenderPass::~RenderPass()
{
    VulkanContext *vkContext = VulkanContext::GetInstance();

    for (auto framebuffer : this->_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(vkContext->logicalDevice, framebuffer, nullptr);
    }

    vkDestroyRenderPass(vkContext->logicalDevice, this->_renderPass, nullptr);
}

void RenderPass::Begin(std::shared_ptr<CommandBuffer> commandBuffer, uint32_t imageIndex)
{
    VulkanContext *vkContext = VulkanContext::GetInstance();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = this->_renderPass;
    renderPassInfo.framebuffer = this->_swapChainFramebuffers[imageIndex];

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = vkContext->swapChainExtent;

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
    VulkanContext *vkContext = VulkanContext::GetInstance();

    this->_swapChainFramebuffers.resize(vkContext->swapChainImageViews.size());

    for (size_t i = 0; i < vkContext->swapChainImageViews.size(); i++)
    {
        VkImageView attachments[] = {
            vkContext->swapChainImageViews[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = this->_renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = vkContext->swapChainExtent.width;
        framebufferInfo.height = vkContext->swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(vkContext->logicalDevice, &framebufferInfo, nullptr, &this->_swapChainFramebuffers[i]) != VK_SUCCESS)
        {
            exit(EXIT_FAILURE);
        }
    }
}

void RenderPass::RecreateFrameBuffers()
{
    VulkanContext *vkContext = VulkanContext::GetInstance();

    for (auto framebuffer : this->_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(vkContext->logicalDevice, framebuffer, nullptr);
    }

    this->CreateFramebuffers();
}
