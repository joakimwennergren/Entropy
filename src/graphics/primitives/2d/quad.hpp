#pragma once

// include Lua headers
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <LuaBridge/LuaBridge.h>

#include "vertex.hpp"
#include "texture.hpp"
#include "context.hpp"
#include "vertexbuffer.hpp"
#include "state.hpp"
#include "renderable.hpp"

using namespace Symbios::Graphics::Textures;
using namespace Symbios::Scripting::States;
using namespace Symbios::Renderables;
using namespace luabridge;

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
                 */
                Quad() = default;

                /**
                 * @brief Construct a new Quad object
                 *
                 * @param context
                 */
                Quad(std::shared_ptr<Context> context);

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