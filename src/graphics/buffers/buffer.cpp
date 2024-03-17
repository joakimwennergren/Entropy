#include "buffer.hpp"

using namespace Entropy::Graphics::Buffers;

Buffer::~Buffer()
{
    ZoneScopedN("Buffer decontructor called");
    // @todo Destroy buffer!!!
    //vmaDestroyBuffer(_allocator->Get(), _buffer, _allocation);
}

/**
 * @brief Create Buffer
 * @param serviceLocator ServiceLocator
 * @param size size of buffer
 * @param usage VkBufferUsageFlags usage flags
 * @return (void)
 */
void Buffer::CreateBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkDeviceSize size, VkBufferUsageFlags usage)
{
    assert(serviceLocator != nullptr);
    assert(size != 0);

    _allocator = serviceLocator->GetService<Allocator>();

    assert(_allocator != nullptr);

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
    allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;

    if (vmaCreateBuffer(_allocator->Get(), &bufferInfo, &allocInfo, &_buffer, &_allocation, nullptr) != VK_SUCCESS)
    {
        spdlog::error("Error while creating buffer with size: {}", size);
    }
}
