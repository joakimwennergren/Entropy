#pragma once

#include "context.hpp"

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

                CommandBuffer()
                {

                };

                CommandBuffer(Symbios::Core::Context *context);
                /**
                 * @brief Destroy the Command Buffer object
                 *
                 */
                ~CommandBuffer();

                /**
                 * @brief
                 *
                 * @param imageIndex
                 * @param renderPass
                 */
                void Record(uint32_t imageIndex);

                /**
                 * @brief
                 *
                 */
                void EndRecording();

                /**
                 * @brief Get the Command Buffer object
                 *
                 * @return VkCommandBuffer
                 */
                VkCommandBuffer GetCommandBuffer() { return this->_commandBuffer; };

            private:
                Context *_context;
                VkCommandPool _commandPool; // @todo should this be here??
                VkCommandBuffer _commandBuffer;
            };
        }
    }
}
