#pragma once
#include "animation/easing/easing.hpp"
#include "box2d/b2_math.h"
#include "cameras/camera_manager.hpp"
#include "ecs/components/objmodel.hpp"
#include "factories/entityfactory.hpp"
#include "filesystem/filesystem.hpp"
#include "physics/2d/physics2d.hpp"
#define SOL_ALL_SAFETIES_ON 1
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

#include "types.hpp"

#include <spdlog/spdlog.h>

#include <sol/sol.hpp>

#include <ecs/world.hpp>
#include <input/mouse/mouse.hpp>

// Bound entities
#include <gltf/model.hpp>
#include <graphics/primitives/2d/sprite.hpp>
#include <graphics/text/font.hpp>
#include <graphics/text/label.hpp>

#include <cameras/orthographic_camera.hpp>
#include <filesystem/filesystem.hpp>

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
using namespace Entropy::Animation;
using namespace Entropy::Filesystem;

using namespace std::chrono_literals;

namespace Entropy {
namespace Scripting {
struct Lua {
  const float PPM = 100.0f; // Pixels Per Meter
public:
  Lua(World world, Factories::EntityFactory ef,
      Cameras::CameraManager cameraManager, Physics::Physics2D physics2d)
      : _world{world}, _entityFactory{ef}, _physics2d{physics2d} {
    _lua = new sol::state();
    _lua->open_libraries(sol::lib::base, sol::lib::table, sol::lib::math);

    _lua->new_usertype<Vector3>("Vector3", "x", &Vector3::x, "y", &Vector3::y,
                                "z", &Vector3::z);

    _lua->new_usertype<b2Vec2>("b2vec2", "x", &b2Vec2::x, "y", &b2Vec2::y);
    _lua->new_usertype<b2Body>("b2Body", "GetPosition", &b2Body::GetPosition,
                               "GetAngle", &b2Body::GetAngle);

    _lua->set_function("create_obj_model",
                       [&ef] { return ef.CreateOBJModel(""); });

    _lua->set_function("create_sprite", [&ef](std::string path) {
      return ef.CreateSprite(path);
    });

    _lua->set_function("translate_3d", [this](flecs::entity entity, float x,
                                              float y, float z) {
      if (!entity.is_alive())
        return;

      auto pos = entity.get_mut<Entropy::Components::Position>();
      pos->pos = glm::vec3(x / PPM, y / PPM, z);
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
                       [this](flecs::entity entity, float x, float y, float z) {
                         if (!entity.is_alive())
                           return;

                         auto s = entity.get_mut<Entropy::Components::Scale>();
                         s->scale = glm::vec3(x / PPM, y / PPM, z / PPM);
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
      spdlog::info("REMOVING ENTITY!!!");
      entity.destruct();
    });

    _lua->set_function("clone", [](flecs::entity entity) -> flecs::entity {
      return entity.clone();
    });

    _lua->set_function("set_zindex", [](flecs::entity entity, int zIndex) {
      if (!entity.is_alive())
        return;

      auto s = entity.get_mut<Entropy::Components::Renderable>();
      s->zIndex = zIndex;
    });

    _lua->set_function("get_position", [](flecs::entity entity) -> Vector3 * {
      if (!entity.is_alive())
        return new Vector3();

      auto s = entity.get_mut<Entropy::Components::Position>();
      auto vec = new Vector3();
      vec->x = s->pos.x;
      vec->y = s->pos.y;
      vec->z = s->pos.z;
      return vec;
    });

    _lua->set_function("get_scale", [](flecs::entity entity) -> Vector3 * {
      if (!entity.is_alive())
        return new Vector3();

      auto s = entity.get_mut<Entropy::Components::Scale>();
      auto vec = new Vector3();
      vec->x = s->scale.x;
      vec->y = s->scale.y;
      vec->z = s->scale.z;
      return vec;
    });

    _lua->set_function("easeInOutCubic", [](float time) {
      return Animation::EasingFunctions::easeInBounce(time);
    });

    _lua->set_function("random_float", [](float min, float max) -> float {
      return min + static_cast<float>(rand()) /
                       (static_cast<float>(RAND_MAX / (max - min)));
    });

    _lua->set_function("random_int", [](int min, int max) -> int {
      return rand() % (max - min + 1) + min;
    });

    _lua->set_function("include", [this](std::string path) {
      _lua->do_file(GetProjectBasePath() + path);
    });

    _lua->set_function("set_ortho_camera_position", [this](float x, float y) {
      auto orthoCamera = dynamic_cast<Cameras::OrthographicCamera *>(
          _cameraManager.currentCamera);
      orthoCamera->setPosition(glm::vec3{x, y, 0.0});
    });

    _lua->set_function(
        "create_dynamic_body", [this](float x, float y, float w, float h) {
          auto dynBody = _physics2d.CreateDynamicBody(x, y, w, h);
          return dynBody;
        });

    _lua->set_function("create_vec2", [this](float x, float y) -> b2Vec2 * {
      auto vec = new b2Vec2();
      vec->x = x;
      vec->y = y;
      return vec;
    });

    _lua->set_function("create_sensor_body", [this](float x, float y, float w,
                                                    float h, b2Vec2 *pos) {
      auto dynBody = _physics2d.CreateSensorBody(x, y, w, h, pos);
      return dynBody;
    });

    _lua->set_function("delete_dynamic_body", [this](b2Body *body) {
      _physics2d.world->DestroyBody(body);
    });

    _lua->set_function("get_dynbody_user_data", [this](b2Body *body) {
      if(body == nullptr)
          return new b2Vec2();
        
      auto userdata = body->GetUserData();
      if(userdata.pointer != 0)
      {
          auto pos = reinterpret_cast<b2Vec2 *>(userdata.pointer);
          return pos;
      }

    });

    _lua->set_function("get_dynbody_linear_velocity", [this](b2Body *body) {
      return body->GetLinearVelocity();
    });

    _lua->set_function("create_static_body",
                       [this](float x, float y, float w, float h) {
                         auto dynBody = _physics2d.CreateGround(x, y, w, h);
                         return dynBody;
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
  Cameras::CameraManager _cameraManager;
  Physics::Physics2D _physics2d;
  int setLuaPath(lua_State *L, const char *path) {
    lua_getglobal(L, "package");
    lua_getfield(L, -1,
                 "path"); // get field "path" from table at top of stack (-1)
    std::string cur_path =
        lua_tostring(L, -1); // grab path string from top of stack
    cur_path.append(";");    // do your path magic here
    cur_path.append(path);
    lua_pop(L,
            1); // get rid of the string on the stack we just pushed on line 5
    lua_pushstring(L, cur_path.c_str()); // push the new one
    lua_setfield(L, -2, "path"); // set the field "path" in table at -2 with
                                 // value at top of stack
    lua_pop(L, 1);               // get rid of package table from top of stack
    return 0;                    // all done!
  }

private:
};
} // namespace Scripting
} // namespace Entropy
