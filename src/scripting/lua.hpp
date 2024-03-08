#pragma once

#include <future>

#include <sol/sol.hpp>

#include <servicelocators/servicelocator.hpp>
#include <scenegraphs/scenegraph.hpp>
#include <ecs/world.hpp>
#include <input/mouse/mouse.hpp>
#include <data/vectors.hpp>
#include <services/service.hpp>

// Bound entities
#include <graphics/primitives/2d/sprite.hpp>
#include <graphics/text/label.hpp>
#include <graphics/text/font.hpp>
#include <gltf/model.hpp>

#include <ecs/components/position.hpp>
#include <ecs/components/scale.hpp>
#include <ecs/components/model.hpp>
#include <ecs/components/gizmo.hpp>
#include <ecs/components/renderable.hpp>
#include <ecs/components/line.hpp>
#include <ecs/components/color.hpp>
#include <ecs/components/boxcollisionshape3d.hpp>
#include <ecs/components/rigidbody3d.hpp>
#include <assetmanagers/assetmanager.hpp>
#include <ecs/components/tags/scripted.hpp>

using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Primitives;
using namespace Entropy::SceneGraphs;
// using namespace Entropy::Input;
using namespace Entropy::Vectors;
using namespace Entropy::Services;
using namespace Entropy::Components;
using namespace Entropy::GLTF;
using namespace Entropy::ECS;
using namespace EntropyEditor;

namespace Entropy
{
    namespace Scripting
    {
        class Lua : public Service
        {
        public:
            Lua(std::shared_ptr<ServiceLocator> serviceLocator);
            inline bool ExecuteScript(std::string script, std::string scriptFile, sol::environment env)
            {
                if (script.length() > 0)
                {
                    _lua.script(script, env);
                }
                if (scriptFile.length() > 0)
                {
                    _lua.script_file(scriptFile, env);
                }

                return true;
            };
            sol::state _lua;

        private:
        };
    }
}
