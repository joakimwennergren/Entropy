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

#include <cstring>
#include <iostream>

using namespace Entropy::Graphics::Vulkan::Buffers;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Buffers {
/**
 * @brief
 *
 */

class StagedBuffer : public Buffer {
public:
  /**
   * @brief Construct a new Buffer object
   *
   * @param context Vulkan context
   */
  StagedBuffer(Vulkan::VulkanBackend backend, VkDeviceSize size, uint8_t *data)
      : Buffer(backend) {

    CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

    void *mappedData;
    vmaMapMemory(_vkBackend.allocator.Get(), _allocation, &mappedData);
    memcpy(mappedData, data, static_cast<size_t>(size));
    vmaUnmapMemory(_vkBackend.allocator.Get(), _allocation);
    // std::memmove(data, dataIn, static_cast<size_t>(size));
    // vmaUnmapMemory(_allocator->Get(), _allocation);
  }

private:
};
} // namespace Buffers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy