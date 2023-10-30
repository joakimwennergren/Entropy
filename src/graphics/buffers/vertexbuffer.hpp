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
            class VertexBuffer : public Buffer
            {
            public:

                VertexBuffer(std::vector<Vertex> vertices);

                ~VertexBuffer();

            private:
            };
        }
    }
}
