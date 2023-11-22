#pragma once

#include <graphics/data/vertex.hpp>
#include <graphics/textures/texture.hpp>
#include <contexts/context.hpp>
#include <graphics/buffers/vertexbuffer.hpp>
#include <renderables/renderable.hpp>

using namespace Symbios::Graphics::Textures;
using namespace Symbios::Renderables;

namespace Symbios
{
    namespace Graphics
    {
        namespace Primitives
        {
            class Quad : public Renderable
            {
            public:
                /**
                 * @brief Construct a new Quad object
                 *
                 * @param context
                 */
                Quad();

                /**
                 * @brief Destroy the Quad object
                 *
                 */
                ~Quad();

                void New(glm::vec3 position, glm::vec3 scale, glm::vec4 color)
                {
                    this->position = position;
                    this->textureId = -1;
                    this->color = color;
                    this->scale = scale;
                }

                void SetPosition(glm::vec3 newPosition) { this->position = newPosition; };

            private:
                std::shared_ptr<Context> _context;
            };
        }
    }
}
