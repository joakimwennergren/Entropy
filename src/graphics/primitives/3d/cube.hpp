#pragma once

#include <graphics/data/vertex.hpp>
#include <graphics/textures/texture.hpp>
#include <graphics/buffers/vertexbuffer.hpp>

using namespace Entropy::Graphics::Textures;

namespace Entropy
{
    namespace Graphics
    {
        namespace Primitives
        {
            class Cube
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
