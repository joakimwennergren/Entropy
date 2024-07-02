#include "commandbuffer.hpp"

using namespace Entropy::Graphics::Vulkan::CommandBuffers;


/**
 * @brief Start recording one-time command buffer
 * @return (void)
 */
void CommandBuffer::RecordOnce() {
  assert(_commandBuffer != VK_NULL_HANDLE);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  if (vkBeginCommandBuffer(_commandBuffer, &beginInfo) != VK_SUCCESS) {
    spdlog::warn(
        "[CommandBuffer] Error while start recording one-time command buffer.");
  }
}

/**
 * @brief Start recording command buffer
 * @return (void)
 */
void CommandBuffer::Record() {
  assert(_commandBuffer != VK_NULL_HANDLE);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;                  // Optional
  beginInfo.pInheritanceInfo = nullptr; // Optional

  if (vkBeginCommandBuffer(_commandBuffer, &beginInfo) != VK_SUCCESS) {
    spdlog::warn("[CommandBuffer] Error while start recording command buffer.");
  }
}

/**
 * @brief Start recording command buffer
 * @return (void)
 */
void CommandBuffer::RecordSecondary(VkRenderPass renderpass) {
  assert(_commandBuffer != VK_NULL_HANDLE);

  VkCommandBufferInheritanceInfo inheritanceInfo = {};
  inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
  inheritanceInfo.renderPass = renderpass;

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;                           // Optional
  beginInfo.pInheritanceInfo = &inheritanceInfo; // Optional

  if (vkBeginCommandBuffer(_commandBuffer, &beginInfo) != VK_SUCCESS) {
    spdlog::warn("[CommandBuffer] Error while start recording command buffer.");
  }
}

/**
 * @brief End recording command buffer
 * @return (void)
 */
void CommandBuffer::EndRecording() {
  assert(_commandBuffer != VK_NULL_HANDLE);

  if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS) {
    spdlog::warn("[CommandBuffer] Error while end recording.");
  }
}

/**
 * @brief End one-time recording
 * @return (void)
 */
void CommandBuffer::EndRecordingOnce() {
  assert(_commandBuffer != VK_NULL_HANDLE);

  if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS) {
    spdlog::warn("[CommandBuffer] Error while end recording once.");
  }
}
