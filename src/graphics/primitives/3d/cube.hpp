#pragma once

#include <graphics/data/vertex.hpp>
#include <graphics/textures/texture.hpp>
#include <graphics/buffers/vertexbuffer.hpp>
#include <renderables/renderable.hpp>

using namespace Entropy::Graphics::Textures;
using namespace Entropy::Renderables;

namespace Entropy
{
    namespace Graphics
    {
        namespace Primitives
        {
            class Cube : public Renderable
            {
            public:
                /**
                 * @brief Construct a new Quad object
                 *
                 * @param context
                 */
                Cube(std::shared_ptr<ServiceLocator> serviceLocator);
            };
        }
    }
}
