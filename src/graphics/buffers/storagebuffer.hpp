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

#include <graphics/buffers/buffer.hpp>
#include <graphics/data/vertex.hpp>

using namespace Entropy::Graphics::Buffers;

namespace Entropy
{
    namespace Graphics
    {
        namespace Buffers
        {
            /**
             * @brief
             *
             */

            class StorageBuffer : public Buffer
            {
            public:
                /**
                 * @brief Construct a new Buffer object
                 *
                 * @param context Vulkan context
                 */
                StorageBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkDeviceSize bufferSize, void *input);

            private:
            };
        }
    }
}
