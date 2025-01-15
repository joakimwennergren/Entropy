#ifndef ENTROPY_COMMAND_BUFFER_H
#define ENTROPY_COMMAND_BUFFER_H

#include <graphics/vulkan/commandpools/commandpool.hpp>
#include <graphics/vulkan/utilities/helpers.hpp>

namespace Entropy::Graphics::Vulkan::CommandBuffers {
 /**
  * @brief Represents a Vulkan CommandBuffer used to record and execute commands.
  *
  * This class provides methods for creating, managing, and recording commands
  * into a Vulkan command buffer. It ensures the proper allocation and cleanup
  * of Vulkan resources, and offers utilities for efficient command buffer usage,
  * including single-use and reusable recording patterns.
  */
 struct CommandBuffer {
  /**
   * @brief CommandBuffer constructor to allocate a Vulkan command buffer.
   * @param level Specifies the level of the allocated command buffer,
   *        which can be either VK_COMMAND_BUFFER_LEVEL_PRIMARY or VK_COMMAND_BUFFER_LEVEL_SECONDARY.
   * @return (void)
   */
  explicit CommandBuffer(VkCommandBufferLevel level);

  /**
   * @brief Destructor for the CommandBuffer class that frees the allocated Vulkan command buffer.
   *        Ensures proper cleanup of resources by releasing the allocated command buffer
   *        from the associated logical device and command pool.
   * @return (void)
   */
  ~CommandBuffer();

  /**
   * @brief Begins recording commands into the Vulkan command buffer.
   *
   * This method sets up the command buffer for recording by initializing
   * a VkCommandBufferBeginInfo structure and calling vkBeginCommandBuffer.
   * It ensures that the command buffer is valid before starting the recording process.
   *
   * @return (void)
   */
  void Record() const;


  /**
   * @brief Ends the recording of commands to this command buffer.
   *
   * This method finalizes the command buffer recording, making it ready for execution.
   * If the command buffer is not in a valid state, a warning will be logged.
   *
   * @return (void)
   */
  void EndRecording() const;

  /**
   * @brief Begins recording of a Vulkan command buffer for a one-time submission.
   *
   * This method sets up a Vulkan command buffer for recording commands that are intended to be submitted
   * only once. It configures the command buffer with VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT.
   * If the command buffer cannot be successfully started for recording, a warning is logged.
   *
   * @return void
   */
  void RecordOnce() const;

  /**
   * @brief Ends the recording of the Vulkan command buffer.
   *
   * This function ensures the command buffer recording is properly terminated.
   * It asserts that the command buffer handle is valid and logs a warning in case of an error during the end recording operation.
   *
   * @return void
   */
  void EndRecordingOnce() const;

  /**
   * @brief Retrieves the currently allocated Vulkan command buffer.
   *
   * This method returns the VkCommandBuffer handle that has been allocated
   * to this CommandBuffer instance.
   *
   * @return VkCommandBuffer The allocated Vulkan command buffer handle.
   */
  [[nodiscard]] VkCommandBuffer Get() const { return _commandBuffer; }

 private:
  VkCommandBuffer _commandBuffer = VK_NULL_HANDLE;
  std::shared_ptr<ILogicalDevice> _logicalDevice;
  std::shared_ptr<ICommandPool> _commandPool;
 };
} // namespace Entropy::Graphics::Vulkan::CommandBuffers

#endif // ENTROPY_COMMAND_BUFFER_H
