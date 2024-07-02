/**
 * @file uniformbuffer.hpp
 * @author Joakim Wennergren (joakim.wennergren@databeams.se)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <graphics/data/vertex.hpp>
#include <graphics/vulkan/buffers/buffer.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Buffers {
/**
 * @brief
 *
 */

struct StorageBuffer : public Buffer {
  /**
   * @brief Construct a new Buffer object
   *
   * @param context Vulkan context
   */
  StorageBuffer(Vulkan::VulkanBackend backend, VkDeviceSize bufferSize,
                void *input)
      : Buffer(backend) {

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    // vkMapMemory(logicalDevice->Get(), _bufferMemory, 0, bufferSize, 0,
    // &_mappedMemory);
  }

private:
};
} // namespace Buffers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
