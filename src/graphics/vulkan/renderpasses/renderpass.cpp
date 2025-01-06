#include "renderpass.hpp"

using namespace Entropy::Graphics::Vulkan::RenderPasses;

void RenderPass::Begin(const CommandBuffer commandBuffer, const uint32_t imageIndex,
                       const int width, const int height) const {
  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = _renderPass;
  renderPassInfo.framebuffer = _frameBuffers[imageIndex];

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent =
      VkExtent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {{1.0f, 1.0f, 1.0f, 1.0f}};
  clearValues[1].depthStencil = {1.0f, 0};

  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffer.Get(), &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
}

void RenderPass::End(const CommandBuffer commandBuffer) {
  vkCmdEndRenderPass(commandBuffer.Get());
}

VkFormat RenderPass::findSupportedFormat(const std::vector<VkFormat> &candidates,
                                         const VkImageTiling tiling,
                                         const VkFormatFeatureFlags features) const {
  for (VkFormat format: candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(_physicalDevice->Get(), format, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
               (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  spdlog::warn("Failed to find supported format.");
  return VK_FORMAT_UNDEFINED;
}

VkFormat RenderPass::FindDepthFormat() const {
  return findSupportedFormat(
    {
      VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
      VK_FORMAT_D24_UNORM_S8_UINT
    },
    VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}
