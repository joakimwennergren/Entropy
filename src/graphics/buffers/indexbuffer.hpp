#pragma once

#include <graphics/buffers/buffer.hpp>

using namespace Entropy::Graphics::Buffers;

namespace Entropy
{
    namespace Graphics
    {
        namespace Buffers
        {
            template <class T>
            class IndexBuffer : public Buffer
            {
            public:
                IndexBuffer(std::shared_ptr<ServiceLocator> serviceLocator, std::vector<T> indices)
                {
                    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
                    assert(bufferSize != 0);

                    CreateBuffer(serviceLocator, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

                    void *mappedData;
                    vmaMapMemory(_allocator->Get(), _allocation, &mappedData);
                    memcpy(mappedData, indices.data(), bufferSize);
                    vmaUnmapMemory(_allocator->Get(), _allocation);
                }

            private:
            };
        }
    }
}
