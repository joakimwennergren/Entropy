/**
 * @file commandbuffer.hpp
 * @author Joakim Wennergren (joakim.wennergren@databeams.se)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <servicelocators/servicelocator.hpp>
#include <graphics/commandpools/commandpool.hpp>
#include <services/service.hpp>
#include <graphics/synchronization/queuesync.hpp>

using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::CommandPools;
using namespace Entropy::Services;
using namespace Entropy::Graphics::Synchronization;

namespace Entropy
{
    namespace Graphics
    {
        namespace CommandBuffers
        {
            /**
             * @brief
             *
             */
            class CommandBuffer
            {
            public:
                /**
                 * @brief Construct a new Command Buffer object
                 *
                 * @param context
                 */
                CommandBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkCommandBufferLevel level);

                /**
                 * @brief
                 *
                 * @param imageIndex
                 */
                void Record();

                /**
                 * @brief
                 *
                 */
                void EndRecording();

                /**
                 * @brief
                 *
                 */
                void RecordOnce();

                /**
                 * @brief
                 *
                 */
                void EndRecordingOnce();

                /**
                 * @brief Get the Command Buffer object
                 *
                 * @return VkCommandBuffer
                 */
                VkCommandBuffer GetCommandBuffer() { return this->_commandBuffer; };

            private:
                VkCommandBuffer _commandBuffer;
                std::shared_ptr<LogicalDevice> _logicalDevice;
                std::shared_ptr<QueueSync> _queueSync;
            };
        }
    }
}
