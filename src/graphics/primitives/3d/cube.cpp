#include "cube.hpp"

using namespace Entropy::Graphics::Primitives;

Cube::Cube()
{

    /*
    _vertices = {

        {-1, -1, 0.5},
        {1, -1, 0.5},
        {-1, 1, 0.5},
        {1, 1, 0.5},
        {-1, -1, -0.5},
        {1, -1, -0.5},
        {-1, 1, -0.5},
        {1, 1, -0.5},
    };
    */

    // _vertices = {
    //     {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    //     {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
    //     {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    //     {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    // this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    // this->textureId = -1;
    // this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // this->scale = glm::vec3(1.0, 1.0, 1.0);

    // vertexBuffer = std::make_unique<VertexBuffer>(serviceLocator, _vertices);
    // indexBuffer = std::make_unique<Buffer>();
    // indexBuffer->CreateIndexBufferUint16(serviceLocator, _indices);
}
