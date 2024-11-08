#pragma once
#include <animation/easing/easing.hpp>
#include <cameras/camera_manager.hpp>
#include <ecs/components/objmodel.hpp>
#include <filesystem/filesystem.hpp>
#include <physics/2d/physics2d.hpp>
#define SOL_ALL_SAFETIES_ON 1

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
#include <ecs/components/hasTexture.hpp>
#include <ecs/components/line.hpp>
#include <ecs/components/model.hpp>
#include <ecs/components/position.hpp>
#include <ecs/components/renderable.hpp>
#include <ecs/components/rigidbody3d.hpp>
#include <ecs/components/rotation.hpp>
#include <ecs/components/scale.hpp>
#include <ecs/components/sprite.hpp>
#include <ecs/components/tags/scripted.hpp>
#include <ecs/components/trianglemeshcollisionshape3d.hpp>

#include <assets/assetid.hpp>
#include <timing/timer.hpp>
#include <tracy/Tracy.hpp>

#include "ilua.hpp"

using namespace Entropy::Graphics::Primitives;
using namespace Entropy::Components;
using namespace Entropy::GLTF;
using namespace Entropy::ECS;
using namespace Entropy::Animation;
using namespace Entropy::Filesystem;
using namespace Entropy::Timing;

using namespace std::chrono_literals;

namespace Entropy::Scripting {
  struct Lua : public ServiceBase<ILua> {
    inline sol::state *Get() override { return _lua; }
    const float PPM = 100.0f; // Pixels Per Meter
  public:
    Lua() {
      ServiceLocator *sl = ServiceLocator::GetInstance();
      _world = sl->getService<IWorld>();
      _physics2d = sl->getService<IPhysics2D>();

      _lua = new sol::state();
      _lua->open_libraries(sol::lib::base, sol::lib::table, sol::lib::math,
                           sol::lib::string, sol::lib::io);
      // _lua->new_usertype<b2Vec2>("b2vec2", "x", &b2Vec2::x, "y", &b2Vec2::y);
      // // _lua->new_usertype<b2BodyId>("b2BodyId", "GetPosition",
      // //                              &b2Body::GetPosition, "GetAngle",
      // //                              &b2Body::GetAngle);

      // _lua->new_usertype<Timer>("Timer", "GetTick", &Timer::GetTick, "Start",
      //                           &Timer::Start, "Reset", &Timer::Reset);

      // _lua->set_function("create_timer", [this](float tick_duration) {
      //   return new Timer(tick_duration);
      // });

      _lua->set_function("create_sprite", [this](const std::string &path) {
        const auto sprite = std::make_shared<Sprite>(
          GetSpritesDir() + path);
        auto e = _world->Get()->entity();
        const auto id = AssetId().GetId();
        e.set<Position>({glm::vec3(0.0, 0.0, 0.0)});
        e.set<Scale>({glm::vec3(25.0, 25.0, 25.0)});
        e.set<Rotation>({glm::vec3(1.0, 1.0, 1.0), 0.0});
        e.set<Entropy::Components::SpriteComponent>({sprite});
        auto renderable = Entropy::Components::Renderable();
        renderable.visible = true;
        renderable.id = id;
        renderable.indexBuffer = sprite->indexBuffer;
        renderable.vertexBuffer = sprite->vertexBuffer;
        renderable.indices = sprite->indices;
        renderable.type = 2;
        return e.set<Entropy::Components::Renderable>(renderable);
        e.set<Entropy::Components::Color>({glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}});
        e.set<Entropy::Components::HasTexture>({sprite->texture});
        return e;
      });

      // _lua->set_function("create_quad", [this]() {
      //   auto quad = std::make_shared<Entropy::Graphics::Primitives::Quad>();
      //   auto e = _world->Get()->entity();
      //   auto id = AssetId().GetId();
      //   e.set<Position>({glm::vec3(0.0, 0.0, 1.0)});
      //   e.set<Scale>({glm::vec3(0.06, 0.06, 1.0)});
      //   e.set<Rotation>({glm::vec3(1.0, 1.0, 1.0), 0.0});
      //   e.set<Entropy::Components::QuadComponent>({quad});
      //   auto renderable = Entropy::Components::Renderable();
      //   renderable.id = id;
      //   renderable.visible = true;
      //   renderable.vertexBuffer = quad->vertexBuffer;
      //   renderable.indexBuffer = quad->indexBuffer;
      //   renderable.indices = quad->indices;
      //   renderable.type = 3;
      //   e.set<Entropy::Components::Renderable>(renderable);
      //   e.set<Entropy::Components::Color>({glm::vec4{1.0f, 0.0f, 1.0f, 1.0f}});
      //   e.set<Entropy::Components::HasTexture>({quad->texture});
      //   return e;
      // });

      _lua->set_function("translate_3d", [this](flecs::entity entity, float x,
                                                float y, float z) {
        if (!entity.is_alive()) {
          return;
        }

        const auto pos = entity.get_mut<Entropy::Components::Position>();
        pos->pos = glm::vec3(x / PPM, y / PPM, z);
      });

      // _lua->set_function("rotate_3d", [](flecs::entity entity, float x, float
      // y,
      //                                    float z, float angle) {
      //   if (!entity.is_alive())
      //     return;

      //   auto rot = entity.get_mut<Entropy::Components::Rotation>();
      //   rot->orientation = glm::vec3(x, y, z);
      //   rot->angle = angle;
      // });

      _lua->set_function("scale_3d",
                         [this](flecs::entity entity, float x, float y, float
                                z) {
                           if (!entity.is_alive())
                             return;

                           auto s =
                               entity.get_mut<Entropy::Components::Scale>();
                           s->scale = glm::vec3(x / PPM, y / PPM, z / PPM);
                         });

      // _lua->set_function("set_color", [](flecs::entity entity, float r, float
      // g,
      //                                    float b, float a) {
      //   if (!entity.is_alive())
      //     return;

      //   auto col = entity.get_mut<Entropy::Components::Color>();
      //   col->color = glm::vec4(r, g, b, a);
      // });

      // _lua->set_function("delete", [](flecs::entity entity) {
      //   if (!entity.is_alive())
      //     return;
      //   spdlog::info("REMOVING ENTITY!!!");
      //   entity.destruct();
      // });

      //   _lua->set_function("clone", [](flecs::entity entity) -> flecs::entity
      //                      { return entity.clone(); });

      //   _lua->set_function("set_zindex", [](flecs::entity entity, int zIndex)
      //                      {
      // if (!entity.is_alive())
      //   return;

      // auto s = entity.get_mut<Entropy::Components::Renderable>();
      // s->zIndex = zIndex; });

      // _lua->set_function("get_position", [](flecs::entity entity) -> b2Vec3 {
      //   if (!entity.is_alive())
      //     return b2Vec3();

      //   auto s = entity.get_mut<Entropy::Components::Position>();
      //   auto vec = b2Vec3();
      //   vec.x = s->pos.x;
      //   vec.y = s->pos.y;
      //   vec.z = s->pos.z;
      //   return vec;
      // });

      // _lua->set_function("get_scale", [](flecs::entity entity) -> b2Vec3 * {
      //   if (!entity.is_alive())
      //     return new b2Vec3();

      //   auto s = entity.get_mut<Entropy::Components::Scale>();
      //   auto vec = new b2Vec3();
      //   vec->x = s->scale.x;
      //   vec->y = s->scale.y;
      //   vec->z = s->scale.z;
      //   return vec;
      // });

      //   _lua->set_function("easeInOutCubic", [](float time)
      //                      { return
      //                      Animation::EasingFunctions::easeInBounce(time); });

      //   _lua->set_function("random_float", [](float min, float max) -> float
      //                      { return min + static_cast<float>(rand()) /
      //                                         (static_cast<float>(RAND_MAX /
      //                                         (max - min))); });

      //   _lua->set_function("random_int", [](int min, int max) -> int
      //                      { return rand() % (max - min + 1) + min; });

      // _lua->set_function("include", [this](std::string path) {
      //   _lua->do_file(GetProjectBasePath() + path);
      // });

      //   _lua->set_function("set_ortho_camera_position", [this](float x, float
      //   y)
      //                      {
      // auto orthoCamera = dynamic_cast<Cameras::OrthographicCamera *>(
      //     _cameraManager.currentCamera);
      // orthoCamera->setPosition(glm::vec3{x, y, 0.0}); });

      // _lua->set_function(
      //     "create_dynamic_body", [this](float x, float y, float w, float h) {
      //       auto dynBody = _physics2d->CreateDynamicBody(x, y, w, h);
      //       return dynBody;
      //     });

      //   _lua->set_function("create_vec2", [this](float x, float y) -> b2Vec2 *
      //                      {
      // auto vec = new b2Vec2();
      // vec->x = x;
      // vec->y = y;
      // return vec; });

      //   _lua->set_function("create_sensor_body", [this](float x, float y, float
      //   w,
      //                                                   float h, b2Vec2 *pos)
      //                      {
      // auto dynBody = _physics2d.CreateSensorBody(x, y, w, h, pos);
      // return dynBody; });

      // _lua->set_function("delete_dynamic_body", [this](b2Body *body) {
      //   _physics2d->Get()->DestroyBody(body);
      // });

      //   _lua->set_function("get_dynbody_user_data", [this](b2Body *body)
      //                      {
      //                        if (body == nullptr)
      //                          return new b2Vec2();

      //                        auto userdata = body->GetUserData();
      //                        if (userdata.pointer != 0)
      //                        {
      //                          auto pos = reinterpret_cast<b2Vec2
      //                          *>(userdata.pointer); return pos;
      //                        } });

      // _lua->set_function("get_dynbody_linear_velocity", [this](b2Body *body) {
      //   return body->GetLinearVelocity();
      // });

      // _lua->set_function("create_static_body",
      //                    [this](float x, float y, float w, float h) {
      //                      auto dynBody = _physics2d->CreateGround(x, y, w, h);
      //                      return dynBody;
      //                    });
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
    std::shared_ptr<IWorld> _world;
    std::shared_ptr<IPhysics2D> _physics2d;

  private
  :
  };
}
