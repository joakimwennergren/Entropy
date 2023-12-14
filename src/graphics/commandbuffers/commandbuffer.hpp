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

#include <global/vulkancontext.hpp>

using namespace Entropy::Global;

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
                CommandBuffer();

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
            };
        }
    }
}
