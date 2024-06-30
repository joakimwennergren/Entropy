#pragma once

#include <spdlog/spdlog.h>

#include <graphics/vulkan/commandpools/commandpool.hpp>
#include <graphics/vulkan/synchronization/queuesync.hpp>

using namespace Entropy::Graphics::Vulkan::CommandPools;
using namespace Entropy::Graphics::Vulkan::Synchronization;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace CommandBuffers {
class CommandBuffer {
public:
  CommandBuffer(VkCommandBufferLevel level);

  void Record();
  void EndRecording();

  void RecordOnce();
  void EndRecordingOnce();

  void RecordSecondary(VkRenderPass renderpass);

  inline VkCommandBuffer GetCommandBuffer() { return _commandBuffer; };

private:
  VkCommandBuffer _commandBuffer = VK_NULL_HANDLE;
  std::shared_ptr<LogicalDevice> _logicalDevice;
  std::shared_ptr<QueueSync> _queueSync;
  std::unique_ptr<CommandPool> _pool;
  VkCommandBufferLevel _level;
};
} // namespace CommandBuffers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy