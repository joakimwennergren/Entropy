#ifndef ENTROPY_BUFFER_H
#define ENTROPY_BUFFER_H

#include <vulkan/vulkan.hpp>
#include <cassert>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/utilities/helpers.hpp>

namespace Entropy::Graphics::Vulkan::Buffers {
 /**
  * Represents a buffer abstraction utilizing Vulkan and the Vulkan Memory Allocator (VMA).
  * Provides functionality to manage Vulkan buffers, including creation, memory allocation, and mapping.
  */
 struct Buffer {
  /**
   * Destructor for the Buffer class.
   * Destroys the buffer and its associated allocation using Vulkan Memory
   * Allocator.
   */
  ~Buffer() {
   vmaDestroyBuffer(_allocator->Get(), _buffer, allocation);
  }

  /**
   * Get the Vulkan buffer associated with this object.
   *
   * @return VkBuffer The Vulkan buffer.
   */
  [[nodiscard]] VkBuffer GetVulkanBuffer() const { return this->_buffer; };

  /**
   * Retrieves the Vulkan device memory associated with the buffer.
   *
   * @return VkDeviceMemory The Vulkan device memory of the buffer.
   */
  [[nodiscard]] VkDeviceMemory GetBufferMemory() const { return this->_bufferMemory; };

  /**
   * Retrieves the pointer to the mapped memory of the buffer.
   *
   * @return void* Pointer to the mapped memory.
   */
  [[nodiscard]] void *GetMappedMemory() const { return this->_mappedMemory; };

  /**
   * Represents a Vulkan Memory Allocator (VMA) allocation.
   */
  VmaAllocation allocation = VK_NULL_HANDLE;

 protected:
  /**
   * Creates a Vulkan buffer with the specified size and usage flags using the Vulkan Memory Allocator.
   *
   * @param size The size of the buffer to create.
   * @param usage The usage flags specifying the allowed usages of the buffer (e.g., transfer source, storage).
   */
  void CreateBuffer(const VkDeviceSize size, const VkBufferUsageFlags usage) {
   const ServiceLocator *sl = ServiceLocator::GetInstance();
   _allocator = sl->getService<Memory::IAllocator>();

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
    &_buffer, &allocation, nullptr));
  }

  // Vulkan Buffer handle
  VkBuffer _buffer = VK_NULL_HANDLE;
  // Buffer memory
  VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;
  // Mapped memory
  void *_mappedMemory = nullptr;
  // Allocator dependency
  std::shared_ptr<Memory::IAllocator> _allocator;
 };
} // namespace Entropy::Graphics::Vulkan::Buffers

#endif // ENTROPY_BUFFER_H
