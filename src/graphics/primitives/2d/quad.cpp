#include "quad.hpp"

using namespace Symbios::Graphics::Primitives;

Quad::Quad(std::shared_ptr<Context> context)
{
    _context = context;


    _indices = {
    0, 1, 2, 2, 3, 0};

    _vertices = {
        {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    texture = new Texture(_context);

    // Create buffers @todo temp!!!
    vertexBuffer = std::make_unique<VertexBuffer>(_context, _vertices);

    indexBuffer = std::make_unique<Buffer>(context);
    indexBuffer->CreateIndexBufferUint16(_indices);
}

Quad::~Quad()
{
}

void Quad::BindToLua()
{
}