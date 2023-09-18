/**
 * @file vertexbuffer.hpp
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
#include "buffer.hpp"
#include "vertex.hpp"

using namespace Symbios::Core;
using namespace Symbios::Graphics::Buffers;

namespace Symbios
{
    namespace Graphics
    {
        namespace Buffers
        {
            /**
             * @brief
             *
             */

            class VertexBuffer : public Buffer
            {
            public:
                /**
                 * @brief Construct a new Buffer object
                 *
                 * @param context Vulkan context
                 */
                VertexBuffer(std::vector<Vertex> vertices);

                /**
                 * @brief Destroy the Buffer object
                 *
                 */
                ~VertexBuffer();

            private:
            };
        }
    }
}
