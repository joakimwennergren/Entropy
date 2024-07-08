#pragma once
#include "ecs/components/objmodel.hpp"
#include "factories/entityfactory.hpp"
#define SOL_ALL_SAFETIES_ON 1
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

#include <spdlog/spdlog.h>

#include <sol/sol.hpp>

#include <ecs/world.hpp>
#include <input/mouse/mouse.hpp>

// Bound entities
#include <gltf/model.hpp>
#include <graphics/primitives/2d/sprite.hpp>
#include <graphics/text/font.hpp>
#include <graphics/text/label.hpp>

#include <ecs/components/boxcollisionshape3d.hpp>
#include <ecs/components/color.hpp>
#include <ecs/components/gizmo.hpp>
#include <ecs/components/line.hpp>
#include <ecs/components/model.hpp>
#include <ecs/components/position.hpp>
#include <ecs/components/renderable.hpp>
#include <ecs/components/rigidbody3d.hpp>
#include <ecs/components/rotation.hpp>
#include <ecs/components/scale.hpp>
#include <ecs/components/tags/scripted.hpp>
#include <ecs/components/trianglemeshcollisionshape3d.hpp>

#include <assets/assetid.hpp>
#include <tracy/Tracy.hpp>

using namespace Entropy::Graphics::Primitives;
using namespace Entropy::Components;
using namespace Entropy::GLTF;
using namespace Entropy::ECS;

using namespace std::chrono_literals;

namespace Entropy {
namespace Scripting {
struct Lua {
public:
  Lua(World world, Factories::EntityFactory ef)
      : _world{world}, _entityFactory{ef} {
    _lua = new sol::state();
    _lua->open_libraries(sol::lib::base);

    _lua->set_function("create_obj_model",
                       [&ef] { return ef.CreateOBJModel(""); });

    _lua->set_function("create_sprite", [&ef](std::string path) {
      return ef.CreateSprite(path);
    });

    _lua->set_function(
        "translate_3d", [](flecs::entity entity, float x, float y, float z) {
          if (!entity.is_alive())
            return;

          auto pos = entity.get_mut<Entropy::Components::Position>();
          pos->pos = glm::vec3(x, y, z);
        });

    _lua->set_function("rotate_3d", [](flecs::entity entity, float x, float y,
                                       float z, float angle) {
      if (!entity.is_alive())
        return;

      auto rot = entity.get_mut<Entropy::Components::Rotation>();
      rot->orientation = glm::vec3(x, y, z);
      rot->angle = angle;
    });

    _lua->set_function("scale_3d",
                       [](flecs::entity entity, float x, float y, float z) {
                         if (!entity.is_alive())
                           return;

                         auto s = entity.get_mut<Entropy::Components::Scale>();
                         s->scale = glm::vec3(x, y, z);
                       });

    _lua->set_function("set_color", [](flecs::entity entity, float r, float g,
                                       float b, float a) {
      if (!entity.is_alive())
        return;

      auto col = entity.get_mut<Entropy::Components::Color>();
      col->color = glm::vec4(r, g, b, a);
    });

    _lua->set_function("delete", [](flecs::entity entity) {
      if (!entity.is_alive())
        return;

      entity.destruct();
    });
  }

  // inline bool ExecuteScript(std::string script, std::string scriptFile,
  //                           sol::environment env) {
  //   if (script.length() > 0) {
  //     _lua.script(script, env);
  //   }
  //   if (scriptFile.length() > 0) {
  //     _lua.script_file(scriptFile, env);
  //   }

  //   return true;
  // };

  // inline void my_panic(sol::optional<std::string> maybe_msg) {
  //   if (maybe_msg) {
  //     const std::string &msg = maybe_msg.value();
  //     spdlog::error(msg);
  //   }
  // }

  // Lua state
  sol::state *_lua;

  // Dependencies
  World _world;
  Factories::EntityFactory _entityFactory;

private:
};
} // namespace Scripting
} // namespace Entropy
