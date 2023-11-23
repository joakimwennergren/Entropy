#pragma once

#include <contexts/context.hpp>
#include <global/vulkancontext.hpp>
#include <graphics/buffers/buffer.hpp>
#include <graphics/data/vertex.hpp>

using namespace Symbios::Core;
using namespace Entropy::Global;
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
