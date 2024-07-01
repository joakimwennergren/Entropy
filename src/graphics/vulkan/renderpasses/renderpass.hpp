#pragma once

#include <graphics/vulkan/devices/logical_device.hpp>
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <factories/vulkan/texturefactory.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>
#include <graphics/vulkan/textures/depthbuffer_texture.hpp>
#include <graphics/vulkan/textures/swapchain_texture.hpp>
#include <vulkan/vulkan.hpp>

#include "spdlog/spdlog.h"

using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::Swapchains;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::Factories::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace RenderPasses {

struct RenderPass {
  RenderPass(VulkanBackend backend, Swapchain swapChain, TextureFactory tf) : _vkBackend{backend}, _swapChain{swapChain}, _textureFactory{tf}
  {

  RecreateDepthBuffer();

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
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = _swapChain.swapChainImageFormat;
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

  if (vkCreateRenderPass(_vkBackend.logicalDevice.Get(), &renderPassInfo, nullptr,
                         &_renderPass) != VK_SUCCESS) {
    spdlog::error("Couldn't create renderpass");
    return;
  }

  // this->CreateFramebuffers();
  }
  // void Begin(std::shared_ptr<CommandBuffer> commandBuffer,
  //            uint32_t imageIndex) const;
  // void End(std::shared_ptr<CommandBuffer> commandBuffer) const;

  // void RecreateFrameBuffers();
  void RecreateDepthBuffer() {
    /*
    if (_depthImage != VK_NULL_HANDLE) {
      vmaDestroyImage(allocator->Get(), _depthImage, _allocation);
    }
    */
    //auto depthBufferTexture = _textureFactory.CreateDepthBufferTexture(_swapChain.swapChainExtent.width, _swapChain.swapChainExtent.height);
    auto depthBufferTexture = _textureFactory.CreateDepthBufferTexture(800, 800);
    _depthBufferTexture = &depthBufferTexture;
  }

  //inline VkRenderPass GetRenderPass() const { return this->_renderPass; };

  std::vector<VkFramebuffer> _frameBuffers;
private:


  std::vector<SwapChainTexture> _swapChainTextures;
  DepthBufferTexture *_depthBufferTexture = nullptr;
  VkRenderPass _renderPass = VK_NULL_HANDLE;



  
  VulkanBackend _vkBackend;
  Swapchain _swapChain;
  TextureFactory _textureFactory;

  VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);
  VkFormat FindDepthFormat();

  void CreateImage(uint32_t width, uint32_t height, VkFormat format,
                   VkImageTiling tiling, VkImageUsageFlags usage);

  void CreateFramebuffers() 
  {
    auto swapChainTexture = _textureFactory.CreateSwapChainTexture(800, 800);
    _swapChainTextures.push_back(swapChainTexture);

    for (size_t i = 0; i < _swapChainTextures.size(); i++) {
      std::array<VkImageView, 2> attachments = {
          _swapChainTextures[i]._swapChainImageView, _depthBufferTexture->_depthImageView};

      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = _renderPass;
      framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
      framebufferInfo.pAttachments = attachments.data();
      framebufferInfo.width = _swapChain.swapChainExtent.width;
      framebufferInfo.height = _swapChain.swapChainExtent.height;
      framebufferInfo.layers = 1;

      if (vkCreateFramebuffer(_vkBackend.logicalDevice.Get(), &framebufferInfo, nullptr,
                              &_frameBuffers[i]) != VK_SUCCESS) {
        spdlog::error("Could not create framebuffers");
        return;
      }
    }
  }
/*




  uint32_t findMemoryType(uint32_t typeFilter,
                          VkMemoryPropertyFlags properties);

  std::vector<VkFramebuffer> _swapChainFramebuffers;

  std::shared_ptr<LogicalDevice> _logicalDevice;
  std::shared_ptr<PhysicalDevice> _physicalDevice;

  VkImage _depthImage = VK_NULL_HANDLE;
  VkImageView depthImageView;
  VmaAllocation _allocation;
  */
};
} // namespace RenderPasses
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
