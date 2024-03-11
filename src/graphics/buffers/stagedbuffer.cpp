#include "stagedbuffer.hpp"

using namespace Entropy::Graphics::Buffers;

StagedBuffer::StagedBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkDeviceSize size, uint8_t *dataIn)
{
    auto logicalDevice = serviceLocator->GetService<LogicalDevice>();

    CreateBuffer(serviceLocator, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

    void *mappedData;
    vmaMapMemory(_allocator->Get(), _allocation, &mappedData);
    memcpy(mappedData, dataIn, static_cast<size_t>(size));
    vmaUnmapMemory(_allocator->Get(), _allocation);
    // std::memmove(data, dataIn, static_cast<size_t>(size));
    // vmaUnmapMemory(_allocator->Get(), _allocation);
}