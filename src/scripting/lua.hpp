#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

#include <spdlog/spdlog.h>

#include <sol/sol.hpp>

#include <servicelocators/servicelocator.hpp>
#include <ecs/world.hpp>
#include <input/mouse/mouse.hpp>
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
#include <ecs/components/rotation.hpp>
#include <ecs/components/trianglemeshcollisionshape3d.hpp>

#include <assetmanagers/assetid.hpp>

using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Primitives;
// using namespace Entropy::Input;
using namespace Entropy::Services;
using namespace Entropy::Components;
using namespace Entropy::GLTF;
using namespace Entropy::ECS;
using namespace EntropyEditor;

using namespace std::chrono_literals;

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

            inline void my_panic(sol::optional<std::string> maybe_msg)
            {
                if (maybe_msg)
                {
                    const std::string &msg = maybe_msg.value();
                    spdlog::error(msg);
                }
            }

            sol::state _lua;
            std::vector<std::shared_future<Entropy::GLTF::Model *>> futures;
            std::vector<flecs::entity> loadedModels;

        private:
        };
    }
}
