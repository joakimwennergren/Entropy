#include "sprite.hpp"

using namespace Symbios::Graphics::Primitives;

Sprite::Sprite()
{
    // Store vulkan ctx
    _context = Contexts::Global::GetInstance()->GetVulkanContext();

    auto _sceneGraph = Contexts::SceneGraph::GetInstance();

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

    this->New(
            Filesystem::GetProjectBasePath() + "/resources/textures/layer_1.png", 
            glm::vec3(1240 * 1.5f, 480 * 2.0f * -1.0f, 0.0),
            glm::vec3(100.0, 100.0, 0.0),
            glm::vec4(1.0, 1.0, 0.0, 1.0)
    );

    for(auto ds : _context->GetDescriptorSets())
    {
        UpdateImage(ds);
    }

    _sceneGraph->renderables.push_back(this);
}

Sprite::~Sprite()
{
    
}
