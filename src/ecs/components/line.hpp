#pragma once

#include <graphics/primitives/2d/line.hpp>

using namespace Entropy::Graphics::Primitives;

namespace Entropy
{
    namespace Components
    {
        struct LineGizmo
        {
            std::shared_ptr<Line> line;
        };
    }
}