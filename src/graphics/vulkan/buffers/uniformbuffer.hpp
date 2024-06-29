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

#include "vulkan_backend.hpp"
#include <graphics/data/vertex.hpp>
#include <graphics/vulkan/buffers/buffer.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Buffers {
/**
 * @brief
 *
 */

class UniformBuffer : public Buffer {
public:
  /**
   * @brief Construct a new Buffer object
   *
   * @param context Vulkan context
   */
  UniformBuffer(Vulkan::VulkanBackend backend, VkDeviceSize bufferSize)
      : Buffer(backend) {
    this->CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

    vmaMapMemory(_vkBackend.allocator.Get(), _allocation, &_mappedMemory);
  }

private:
};
} // namespace Buffers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
