#pragma once

#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/devices/logical_device.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>
#include <graphics/vulkan/textures/depthbuffer_texture.hpp>
#include <graphics/vulkan/textures/swapchain_texture.hpp>
#include <vulkan/vulkan.hpp>

#include "graphics/vulkan/utilities/utilities.hpp"
#include "spdlog/spdlog.h"

using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::Swapchains;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::Memory;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace RenderPasses {

struct RenderPass {
  RenderPass() {
    ServiceLocator *sl = ServiceLocator::GetInstance();
    _logicalDevice = sl->getService<ILogicalDevice>();
    _physicalDevice = sl->getService<IPhysicalDevice>();
    _swapchain = sl->getService<ISwapchain>();

    assert(_logicalDevice != nullptr);
    assert(_physicalDevice != nullptr);

    RecreateDepthBuffer(_swapchain->swapChainExtent.width,
                        _swapchain->swapChainExtent.height);

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = FindDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VK_FORMAT_B8G8R8A8_UNORM;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkSubpassDependency dependency{};
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                               VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment,
                                                          depthAttachment};
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    VK_CHECK(vkCreateRenderPass(_logicalDevice->Get(), &renderPassInfo, nullptr,
                                &_renderPass));
  }

  void Begin(CommandBuffer commandBuffer, uint32_t imageIndex, int width,
             int height);

  void End(CommandBuffer commandBuffer);

  void RecreateFrameBuffers(int width, int height, bool app) {
    for (auto framebuffer : _frameBuffers) {
      vkDestroyFramebuffer(_logicalDevice->Get(), framebuffer, nullptr);
    }
    if (app)
      CreateFramebuffers(_swapchain, width, height);
    else
      CreateFramebuffers(width, height);
  }

  void RecreateDepthBuffer(int width, int height) {
    _depthBufferTexture = std::make_shared<DepthBufferTexture>(width, height);
  }

  inline VkRenderPass Get() { return _renderPass; };

  std::vector<VkFramebuffer> _frameBuffers;
  std::vector<std::shared_ptr<SwapChainTexture>> _swapChainTextures;

  void CreateFramebuffers(int width, int height) {
    _swapChainTextures.clear();
    _frameBuffers.clear();
    _swapChainTextures.push_back(
        std::make_shared<SwapChainTexture>(width, height));
    _frameBuffers.resize(1);

    std::array<VkImageView, 2> attachments = {
        _swapChainTextures[0]->imageView->Get(),
        _depthBufferTexture->imageView->Get()};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = _renderPass;
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = width;
    framebufferInfo.height = height;
    framebufferInfo.layers = 1;

    VkResult res = vkCreateFramebuffer(_logicalDevice->Get(), &framebufferInfo,
                                       nullptr, &_frameBuffers[0]);
  }

  void CreateFramebuffers(std::shared_ptr<ISwapchain> swapChain, int width,
                          int height) {

    //_swapChainTextures.clear();
    _frameBuffers.clear();

    // auto swapChainTexture =
    //     _textureFactory.CreateSwapChainTexture(width, height);

    // auto swapChainTexture2 =
    //     _textureFactory.CreateSwapChainTexture(width, height);

    // _swapChainTextures.push_back(swapChainTexture);
    // _swapChainTextures.push_back(swapChainTexture2);

    _frameBuffers.resize(3);

    for (size_t i = 0; i < swapChain->_swapChainImageViews.size(); i++) {
      std::array<VkImageView, 2> attachments = {
          swapChain->_swapChainImageViews[i],
          _depthBufferTexture->imageView->Get()};

      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = _renderPass;
      framebufferInfo.attachmentCount =
          static_cast<uint32_t>(attachments.size());
      framebufferInfo.pAttachments = attachments.data();
      framebufferInfo.width = width;
      framebufferInfo.height = height;
      framebufferInfo.layers = 1;

      VkResult res = vkCreateFramebuffer(
          _logicalDevice->Get(), &framebufferInfo, nullptr, &_frameBuffers[i]);
    }
  }

private:
  std::shared_ptr<DepthBufferTexture> _depthBufferTexture = nullptr;
  VkRenderPass _renderPass = VK_NULL_HANDLE;

  VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);
  VkFormat FindDepthFormat();

  std::shared_ptr<ILogicalDevice> _logicalDevice;
  std::shared_ptr<IPhysicalDevice> _physicalDevice;
  std::shared_ptr<ISwapchain> _swapchain;
};

} // namespace RenderPasses
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
