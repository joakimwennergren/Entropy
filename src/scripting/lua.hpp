#pragma once

#include <sol/sol.hpp>

#include <servicelocators/servicelocator.hpp>
#include <scenegraphs/scenegraph.hpp>
#include <input/mouse/mouse.hpp>
#include <data/vectors.hpp>

// Bound entities
#include <graphics/primitives/2d/sprite.hpp>

#include <physics/2d/physics2d.hpp>

using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Primitives;
using namespace Entropy::SceneGraphs;
using namespace Entropy::Physics;
using namespace Entropy::Input;
using namespace Entropy::Vectors;

namespace Entropy
{
    namespace Scripting
    {
        class Lua
        {
        public:
            Lua(std::shared_ptr<ServiceLocator> serviceLocator);
            sol::state lua;

        private:
        };
    }
}
