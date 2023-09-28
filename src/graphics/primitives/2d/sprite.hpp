#pragma once

#include "vertex.hpp"
#include "texture.hpp"
#include "context.hpp"
#include "vertexbuffer.hpp"
#include "renderable.hpp"
#include "scenegraph.hpp"
#include "filesystem.hpp"

using namespace Symbios::Graphics::Textures;
using namespace Symbios::Renderables;

namespace Symbios
{
    namespace Graphics
    {
        namespace Primitives
        {
            class Sprite: public Renderable
            {
            public:
                /**
                 * @brief Construct a new Quad object
                 *
                 * @param context
                 */
                Sprite();

                /**
                 * @brief Destroy the Quad object
                 *
                 */
                ~Sprite();

                void New(std::string path, glm::vec3 position, glm::vec3 scale, glm::vec4 color)
                {
                    this->position = position;
                    this->textureId = 1;
                    this->color = color;
                    this->scale = scale;
                    this->texture->CreateTextureImage(path);
                }

                void SetPosition(glm::vec3 newPosition) { this->position = newPosition; };

                std::vector<Vertex> _vertices = {
                    {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
                    {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
                    {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
                    {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

            private:

            };
        }
    }
}
