#include "sprite.hpp"

using namespace Symbios::Graphics::Primitives;

Sprite::Sprite()
{
     // Store vulkan ctx
    _context = Contexts::Global::GetInstance()->GetVulkanContext();

    _indices = {
    0, 1, 2, 2, 3, 0};

    _vertices = {
        {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    texture = new Texture();

    // Create buffers @todo temp!!!
    vertexBuffer = std::make_shared<VertexBuffer>(_vertices);
    indexBuffer = std::make_shared<Buffer>();
    
    indexBuffer->CreateIndexBufferUint16(_indices); 

    _descriptorSetLayout = nullptr;
}

Sprite::Sprite(FT_Bitmap bitmap)
{
     // Store vulkan ctx
    _context = Contexts::Global::GetInstance()->GetVulkanContext();

    _indices = {
    0, 1, 2, 2, 3, 0};

    _vertices = {
        {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    texture = new Texture();

    // Create buffers @todo temp!!!
    vertexBuffer = std::make_shared<VertexBuffer>(_vertices);
    indexBuffer = std::make_shared<Buffer>();

    indexBuffer->CreateIndexBufferUint16(_indices);  

    texture->CreateTextureImageFromBuffer(bitmap);
}


Sprite::Sprite(std::string path)
{
    // Store vulkan ctx
    _context = Contexts::Global::GetInstance()->GetVulkanContext();

    _indices = {
    0, 1, 2, 2, 3, 0};

    _vertices = {
        {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    texture = new Texture();

    // Create buffers @todo temp!!!
    vertexBuffer = std::make_shared<VertexBuffer>(_vertices);

    indexBuffer = std::make_shared<Buffer>();
    indexBuffer->CreateIndexBufferUint16(_indices);

    this->New(
            Filesystem::GetProjectBasePath() + "/" + path, 
            glm::vec3(1240 * 1.5f, 480 * 2.0f * -1.0f, 0.0),
            glm::vec3(0.0, 0.0, 0.0),
            glm::vec4(1.0, 1.0, 1.0, 1.0)
    );
    
    UpdateImage();
}

Sprite::~Sprite()
{
    PLOG_DEBUG << "SPRITE DESCRUCTOR CALLED!";
}
