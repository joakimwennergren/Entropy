#pragma once

#include <graphics/primitives/3d/plane.hpp>

using namespace Entropy::Graphics::Primitives;

namespace Entropy
{
    namespace Components
    {
        struct Gizmo
        {
            std::shared_ptr<Plane> plane;
        };
    }
}