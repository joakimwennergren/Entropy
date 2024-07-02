#pragma once

#include <spdlog/spdlog.h>

#include <graphics/vulkan/commandpools/commandpool.hpp>
#include <graphics/vulkan/synchronization/queuesync.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>

using namespace Entropy::Graphics::Vulkan::CommandPools;
using namespace Entropy::Graphics::Vulkan::Synchronization;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace CommandBuffers {
class CommandBuffer {
public:
  CommandBuffer(VulkanBackend vbe, QueueSync qs, CommandPool cp, VkCommandBufferLevel level) : _vkBackend{vbe}, _queueSync{qs}, _commandPool{cp}, _level{level}
  {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _commandPool.Get();
    allocInfo.level = level;
    allocInfo.commandBufferCount = 1;
    if (vkAllocateCommandBuffers(_vkBackend.logicalDevice.Get(), &allocInfo,
                                &_commandBuffer) != VK_SUCCESS) {
      spdlog::warn("[CommandBuffer] Failed to allocate command buffer.");
    }
  }

  void Record();
  void EndRecording();

  void RecordOnce();
  void EndRecordingOnce();

  void RecordSecondary(VkRenderPass renderpass);

  inline VkCommandBuffer GetCommandBuffer() { return _commandBuffer; };

private:
  VkCommandBuffer _commandBuffer = VK_NULL_HANDLE;
  VulkanBackend _vkBackend;
  QueueSync _queueSync;
  CommandPool _commandPool;
  VkCommandBufferLevel _level;
};
} // namespace CommandBuffers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy