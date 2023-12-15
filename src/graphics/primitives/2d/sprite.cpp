#include "sprite.hpp"

using namespace Symbios::Graphics::Primitives;

Sprite::Sprite()
{
    /*
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
    */
}

Sprite::Sprite(FT_Bitmap bitmap)
{
    /*
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

    texture->CreateTextureImageFromBuffer(bitmap);

    UpdateImage();
    */
}

Sprite::Sprite(std::shared_ptr<ServiceLocator> serviceLocator, std::string path)
{
    _indices = {0, 1, 2, 2, 3, 0};

    _vertices = {
        {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    texture = new Texture(serviceLocator);

    // Create buffers @todo temp!!!
    vertexBuffer = std::make_unique<VertexBuffer>(serviceLocator, _vertices);

    indexBuffer = std::make_unique<Buffer>();
    indexBuffer->CreateIndexBufferUint16(serviceLocator, _indices);

    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->textureId = 1;
    this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->scale = glm::vec3(1.0, 1.0, 1.0);
    this->texture->CreateTextureImage(path);

    UpdateImage();
}

Sprite::Sprite(unsigned char *pixels, int width, int height)
{
    /*
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

    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->textureId = 1;
    this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->scale = glm::vec3(1.0, 1.0, 1.0);
    this->texture->CreateTextureImageFromPixels(pixels, width, height);

    UpdateImage();
    */
}