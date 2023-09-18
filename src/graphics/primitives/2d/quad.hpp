#pragma once

#include "vertex.hpp"
#include "texture.hpp"
#include "context.hpp"
#include "vertexbuffer.hpp"
#include "state.hpp"
#include "renderable.hpp"

using namespace Symbios::Graphics::Textures;
using namespace Symbios::Scripting::States;
using namespace Symbios::Renderables;

namespace Symbios
{
    namespace Graphics
    {
        namespace Primitives
        {
            class Quad: public Renderable
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

                void BindToLua();

                void Test() { PLOG_ERROR << "KYK"; };

                void SetPosition(glm::vec3 newPosition) { this->position = newPosition; };


            private:
                std::shared_ptr<Context> _context;

            };
        }
    }
}
