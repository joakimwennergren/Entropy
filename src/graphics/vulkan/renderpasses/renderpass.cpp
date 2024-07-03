#include "renderpass.hpp"
#include "vulkan/vulkan_core.h"

using namespace Entropy::Graphics::Vulkan::RenderPasses;

// RenderPass::RenderPass() {
//  // Get required depenencies
//  auto swapChain = serviceLocator->GetService<Swapchain>();
//  auto logicalDevice = serviceLocator->GetService<LogicalDevice>();
//  auto physicalDevice = serviceLocator->GetService<PhysicalDevice>();

// // Store swapchain and logical device
// _swapChain = swapChain;
// _logicalDevice = logicalDevice;
// _physicalDevice = physicalDevice;
// _serviceLocator = serviceLocator;

// RecreateDepthBuffer();

// VkAttachmentDescription depthAttachment{};
// depthAttachment.format = FindDepthFormat();
// depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
// depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
// depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
// depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
// depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
// depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
// depthAttachment.finalLayout =
//     VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

// VkAttachmentReference depthAttachmentRef{};
// depthAttachmentRef.attachment = 1;
// depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

// VkAttachmentDescription colorAttachment{};
// // colorAttachment.format = swapChain->swapChainImageFormat;
// colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
// colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
// colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
// colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
// colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

// VkSubpassDependency dependency{};
// dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
//                           VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
// dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
//                           VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
// dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
//                            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
// dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

// VkAttachmentReference colorAttachmentRef{};
// colorAttachmentRef.attachment = 0;
// colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

// VkSubpassDescription subpass{};
// subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
// subpass.colorAttachmentCount = 1;
// subpass.pColorAttachments = &colorAttachmentRef;
// subpass.pDepthStencilAttachment = &depthAttachmentRef;

// std::array<VkAttachmentDescription, 2> attachments = {colorAttachment,
//                                                       depthAttachment};
// VkRenderPassCreateInfo renderPassInfo{};
// renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
// renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
// renderPassInfo.pAttachments = attachments.data();
// renderPassInfo.subpassCount = 1;
// renderPassInfo.pSubpasses = &subpass;
// renderPassInfo.dependencyCount = 1;
// renderPassInfo.pDependencies = &dependency;

// // if (vkCreateRenderPass(logicalDevice->Get(), &renderPassInfo, nullptr,
// //                        &_renderPass) != VK_SUCCESS) {
// //   spdlog::error("Couln't create renderpass");
// //   return;
// // }

// this->CreateFramebuffers();
//}

// RenderPass::~RenderPass() {

//   for (auto framebuffer : this->_swapChainFramebuffers) {
//     vkDestroyFramebuffer(_logicalDevice->Get(), framebuffer, nullptr);
//   }

//   vkDestroyRenderPass(_logicalDevice->Get(), _renderPass, nullptr);
// }

void RenderPass::Begin(CommandBuffer commandBuffer, uint32_t imageIndex,
                       int width, int height) const {

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = _renderPass;
  renderPassInfo.framebuffer = _frameBuffers[imageIndex];

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent =
      VkExtent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {{0.05f, 0.05f, 0.05f, 1.0f}};
  clearValues[1].depthStencil = {1.0f, 0};

  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffer.Get(), &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
}

void RenderPass::End(CommandBuffer commandBuffer) const {
  vkCmdEndRenderPass(commandBuffer.Get());
}

// /**
//  * @brief Create frame buffers
//  * @return (void)
//  */
// void RenderPass::CreateFramebuffers() {
//   this->_swapChainFramebuffers.resize(_swapChain->swapChainImageViews.size());

//   for (size_t i = 0; i < _swapChain->swapChainImageViews.size(); i++) {
//     std::array<VkImageView, 2> attachments = {
//         _swapChain->swapChainImageViews[i], depthImageView};

//     VkFramebufferCreateInfo framebufferInfo{};
//     framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//     framebufferInfo.renderPass = _renderPass;
//     framebufferInfo.attachmentCount =
//     static_cast<uint32_t>(attachments.size()); framebufferInfo.pAttachments =
//     attachments.data(); framebufferInfo.width =
//     _swapChain->swapChainExtent.width; framebufferInfo.height =
//     _swapChain->swapChainExtent.height; framebufferInfo.layers = 1;

//     if (vkCreateFramebuffer(_logicalDevice->Get(), &framebufferInfo, nullptr,
//                             &_swapChainFramebuffers[i]) != VK_SUCCESS) {
//       spdlog::error("Could not create framebuffers");
//       return;
//     }
//   }
// }

// /**
//  * @brief
//  * @todo Dont destroy all the framebuffers at once..
//  * @return (void)
//  */
// void RenderPass::RecreateFrameBuffers() {
//   for (auto framebuffer : _swapChainFramebuffers) {
//     vkDestroyFramebuffer(_logicalDevice->Get(), framebuffer, nullptr);
//   }

//   this->CreateFramebuffers();
// }

// /**
//  * @brief Find supported format among candidates
//  * @param candidates vector<VkFormat> candidates
//  * @param tiling VkImageTiling tiling
//  * @param features VkFormatFeatureFlags flags
//  * @return VkFormat supported format
//  */
VkFormat
RenderPass::findSupportedFormat(const std::vector<VkFormat> &candidates,
                                VkImageTiling tiling,
                                VkFormatFeatureFlags features) {

  for (VkFormat format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(_vkBackend.physicalDevice.Get(), format,
                                        &props);

    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
               (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  spdlog::warn("Failed to find supported format.");
}

// /**
//  * @brief Find depth color format
//  * @return  VKFormat depth color format
//  */
VkFormat RenderPass::FindDepthFormat() {
  return findSupportedFormat(
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
       VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

// /**
//  * @brief Create frame buffer image
//  * @param width uint32_t width of framebuffer image
//  * @param height uint32_t height of framebuffer image
//  * @param format VkFormat color format
//  * @param tiling VkImageTiling tiling
//  * @param usage VkImageUsageFlags flags
//  * @return (void)
//  */
void RenderPass::CreateImage(uint32_t width, uint32_t height, VkFormat format,
                             VkImageTiling tiling, VkImageUsageFlags usage) {

  // VmaAllocationCreateInfo allocCreateInfo = {};
  // allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
  // allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

  // VkImageCreateInfo imageInfo{};
  // imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  // imageInfo.imageType = VK_IMAGE_TYPE_2D;
  // imageInfo.extent.width = width;
  // imageInfo.extent.height = height;
  // imageInfo.extent.depth = 1;
  // imageInfo.mipLevels = 1;
  // imageInfo.arrayLayers = 1;
  // imageInfo.format = format;
  // imageInfo.tiling = tiling;
  // imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  // imageInfo.usage = usage;
  // imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  // imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  // VkResult res = vmaCreateImage(allocator->Get(), &imageInfo,
  // &allocCreateInfo,
  //     &_depthImage, &_allocation, nullptr);
}

// /**
//  * @brief (Re)Create depth buffer
//  * @return (void)
//  */
