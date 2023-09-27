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

#include "context.hpp"
#include "global.hpp"

using namespace Symbios::Core;

namespace Symbios
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
                 * @brief Destroy the Command Buffer object
                 *
                 */
                ~CommandBuffer();

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
                std::shared_ptr<Context> _context;
                VkCommandBuffer _commandBuffer;
            };
        }
    }
}
