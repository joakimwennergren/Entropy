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
            class VertexBuffer : public Buffer
            {
            public:
                VertexBuffer(std::shared_ptr<ServiceLocator> serviceLocator, std::vector<Vertex> vertices);

            private:
            };
        }
    }
}
