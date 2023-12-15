#include "quad.hpp"

using namespace Symbios::Graphics::Primitives;

Quad::Quad()
{
    _indices = {
        0, 1, 2, 2, 3, 0};

    _vertices = {
        {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    // Create buffers @todo temp!!!
    // vertexBuffer = std::make_unique<VertexBuffer>(_vertices);

    // indexBuffer = std::make_unique<Buffer>();
    //  indexBuffer->CreateIndexBufferUint16(_indices);
}

Quad::~Quad()
{
}
