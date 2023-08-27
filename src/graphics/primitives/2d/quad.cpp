#include "quad.hpp"

using namespace Symbios::Graphics::Primitives;

Quad::Quad(std::shared_ptr<Context> context)
{
    _context = context;

    texture = std::make_unique<Texture>(_context);

    // Create buffers @todo temp!!!
    vertexBuffer = std::make_unique<Buffer>(context);
    vertexBuffer->CreateVertexBuffer(_vertices);

    indexBuffer = std::make_unique<Buffer>(context);
    indexBuffer->CreateIndexBufferUint16(_indices);
}

Quad::~Quad()
{
}