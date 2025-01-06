#include "commandbuffer.hpp"

using namespace Entropy::Graphics::Vulkan::CommandBuffers;

void CommandBuffer::RecordOnce() const {
  assert(_commandBuffer != VK_NULL_HANDLE);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  VK_CHECK(vkBeginCommandBuffer(_commandBuffer, &beginInfo));
}

void CommandBuffer::Record() const {
  assert(_commandBuffer != VK_NULL_HANDLE);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0; // Optional
  beginInfo.pInheritanceInfo = nullptr; // Optional

  VK_CHECK(vkBeginCommandBuffer(_commandBuffer, &beginInfo));
}

void CommandBuffer::EndRecording() const {
  assert(_commandBuffer != VK_NULL_HANDLE);
  VK_CHECK(vkEndCommandBuffer(_commandBuffer));
}

void CommandBuffer::EndRecordingOnce() const {
  assert(_commandBuffer != VK_NULL_HANDLE);
  VK_CHECK(vkEndCommandBuffer(_commandBuffer));
}
