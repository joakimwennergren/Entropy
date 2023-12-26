#pragma once

#include <sol/sol.hpp>

#include <servicelocators/servicelocator.hpp>
#include <scenegraphs/scenegraph.hpp>
#include <input/mouse/mouse.hpp>
#include <data/vectors.hpp>
#include <services/service.hpp>

// Bound entities
#include <graphics/primitives/2d/sprite.hpp>

#include <physics/2d/physics2d.hpp>

using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Primitives;
using namespace Entropy::SceneGraphs;
using namespace Entropy::Physics;
using namespace Entropy::Input;
using namespace Entropy::Vectors;
using namespace Entropy::Services;

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
                    lua.script(script, env);
                    auto test = lua["Test"];
                    test();
                }
                if (scriptFile.length() > 0)
                {
                    lua.script_file(scriptFile, env);
                    auto test = lua["Test"];
                    test();
                }
            };
            inline bool isValid() override { return true; };
            sol::state lua;

        private:
        };
    }
}
