#pragma once

#include <spdlog/spdlog.h>

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
            class CommandBuffer
            {
            public:
                CommandBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkCommandBufferLevel level);

                void Record();
                void EndRecording();

                void RecordOnce();
                void EndRecordingOnce();

                VkCommandBuffer GetCommandBuffer() { return this->_commandBuffer; };

            private:
                VkCommandBuffer _commandBuffer = VK_NULL_HANDLE;
                std::shared_ptr<LogicalDevice> _logicalDevice;
                std::shared_ptr<QueueSync> _queueSync;
                std::unique_ptr<CommandPool> _pool;
            };
        }
    }
}
