#pragma once

#include <global/vulkancontext.hpp>
#include <graphics/buffers/buffer.hpp>
#include <graphics/data/vertex.hpp>

using namespace Entropy::Global;
using namespace Entropy::Graphics::Buffers;

namespace Entropy
{
    namespace Graphics
    {
        namespace Buffers
        {
            class VertexBuffer : public Buffer
            {
            public:
                VertexBuffer(std::vector<Vertex> vertices);

            private:
            };
        }
    }
}
