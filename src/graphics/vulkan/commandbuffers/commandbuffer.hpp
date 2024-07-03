#pragma once

#include <graphics/vulkan/commandpools/commandpool.hpp>
#include <graphics/vulkan/synchronization/queuesync.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>
#include <spdlog/spdlog.h>

using namespace Entropy::Graphics::Vulkan::CommandPools;
using namespace Entropy::Graphics::Vulkan::Synchronization;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace CommandBuffers {

/**
 * @brief CommandBuffer
 * @author Joakim Wennergren
 * @since Wed Jul 03 2024
 */
struct CommandBuffer {
public:
  /**
   * @brief Constructor for CommandBuffer
   * @param vbe VulkanBackend
   * @param qs QueueSync
   * @param cp CommandPool
   * @param level VkCommandBufferLevel
   */
  CommandBuffer(VulkanBackend vbe, QueueSync qs, CommandPool cp,
                VkCommandBufferLevel level)
      : _vkBackend{vbe}, _queueSync{qs}, _commandPool{cp}, _level{level} {

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

  /**
   * @brief Start recording commandbuffer
   * @return (void)
   */
  void Record();

  /**
   * @brief End recording commandbuffer
   * @return (void)
   */
  void EndRecording();

  /**
   * @brief Record once
   * @return (void)
   */
  void RecordOnce();

  /**
   * @brief End Record once
   * @return (void)
   */
  void EndRecordingOnce();

  /**
   * @brief Get the command buffer handle
   * @return VkCommandBuffer
   */
  inline VkCommandBuffer Get() { return _commandBuffer; };

private:
  // CommandBuffer handle
  VkCommandBuffer _commandBuffer = VK_NULL_HANDLE;

  // Dependencies
  VulkanBackend _vkBackend;
  QueueSync _queueSync;
  CommandPool _commandPool;
  VkCommandBufferLevel _level;
};
} // namespace CommandBuffers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy