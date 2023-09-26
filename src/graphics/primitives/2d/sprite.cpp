#include "sprite.hpp"

using namespace Symbios::Graphics::Primitives;

Sprite::Sprite()
{
    // Store vulkan ctx
    _context = Global::GetInstance()->GetVulkanContext();

    _indices = {
    0, 1, 2, 2, 3, 0};

    _vertices = {
        {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    texture = new Texture();

    // Create buffers @todo temp!!!
    vertexBuffer = std::make_unique<VertexBuffer>(_vertices);

    indexBuffer = std::make_unique<Buffer>();
    indexBuffer->CreateIndexBufferUint16(_indices);
}

Sprite::~Sprite()
{
    
}