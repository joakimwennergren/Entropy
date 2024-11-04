#pragma once

#include <cassert>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

using namespace Entropy::Graphics::Vulkan::Memory;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Buffers {

struct Buffer {

  /**
   * @brief Buffer destructor, destroy the created buffer
   */
  ~Buffer() { vmaDestroyBuffer(_allocator->Get(), _buffer, _allocation); }

  /**
   * @brief Get the basic vulka buffer
   * @return VkBuffer the vulkan buffer
   */
  inline VkBuffer GetVulkanBuffer() { return this->_buffer; };

  /**
   * @brief Get buffer memory
   * @return VkDeviceMemory
   */
  inline VkDeviceMemory GetBufferMemory() { return this->_bufferMemory; };

  /**
   * @brief Get the mapped memory
   * @return void* mapped memory
   */
  inline void *GetMappedMemory() { return this->_mappedMemory; };

  // Allocation
  VmaAllocation _allocation = VK_NULL_HANDLE;

protected:
  /**
   * @brief Create a buffer
   * @param size buffer size
   * @param usage buffer usage flags
   * @return (void)
   */
  inline void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage) {

    ServiceLocator *sl = ServiceLocator::GetInstance();
    _allocator = sl->getService<IAllocator>();

    assert(size != 0);
    assert(usage != 0);

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
    allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;

    VK_CHECK(vmaCreateBuffer(_allocator->Get(), &bufferInfo, &allocInfo,
                             &_buffer, &_allocation, nullptr));
  }

  // Vulkan Buffer handle
  VkBuffer _buffer = VK_NULL_HANDLE;
  // Buffer memory
  VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;
  // Mapped memory
  void *_mappedMemory = nullptr;
  // Allocator dependency
  std::shared_ptr<IAllocator> _allocator;
};

} // namespace Buffers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
