#pragma once


#include <animation/easing/easing.hpp>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <graphics/primitives/primitives.hpp>
#include <animation/easing/easing.hpp>
#include <filesystem/filesystem.hpp>
#include <physics/2d/physics2d.hpp>
#include <config.hpp>


#include <gltf/model.hpp>
#include <cameras/orthographic_camera.hpp>
#include <ecs/components/model.hpp>
#include <ecs/components/position.hpp>
#include <ecs/components/scale.hpp>
#include <timing/timer.hpp>

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
  struct Lua final : ServiceBase<ILua> {
    /**
     * @brief Initializes the Lua scripting environment and sets up various Lua functions for interaction with the world.
     *
     * This constructor sets up the Lua state, opens necessary Lua libraries, and binds various custom C++ functions and components
     * to be used within Lua scripts, enabling interaction with the application's world and entities.
     *
     * @return A new instance of the Lua class.
     */
    Lua() {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      _world = sl->getService<IWorld>();
      _physics2d = sl->getService<IPhysics2D>();
      _cameraManager = sl->getService<ICameraManager>();

      _lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::math,
                          sol::lib::string, sol::lib::io, sol::lib::os, sol::lib::package);

      // Set the Lua module path to include your current working directory
      const std::string base_path = ENGINE_BASEPATH;
      const std::string cwd = base_path + "/lua";
      _lua["package"]["path"] = cwd + "/?.lua;" + _lua["package"]["path"].get<std::string>();

      BindTypes();
      BindFunctions();


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

      // _lua->set_function("include", [this](std::string path) {
      //   _lua->do_file(GetProjectBasePath() + path);
      // });

      //   _lua->set_function("set_ortho_camera_position", [this](float x, float
      //   y)
      //                      {
      // auto orthoCamera = dynamic_cast<Cameras::OrthographicCamera *>(
      //     _cameraManager.currentCamera);
      // orthoCamera->setPosition(glm::vec3{x, y, 0.0}); });


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

    void SetBasePath(const std::string &path) override {
      _lua["package"]["path"] = path + "/?.lua;" + _lua["package"]["path"].get<std::string>();
    }


    void BindTypes() {
      _lua.new_usertype<b2BodyId>("b2BodyId");
      //_lua.new_usertype<b2Vec2>("b2Vec2", "x", &b2Vec2::x, "y", &b2Vec2::y);

      // Box2D 2D vector
      //

      // _lua->new_usertype<Timer>("Timer", "GetTick", &Timer::GetTick, "Start",
      //                           &Timer::Start, "Reset", &Timer::Reset);

      // _lua->set_function("create_timer", [this](float tick_duration) {
      //   return new Timer(tick_duration);
      // });
    }

    /**
     * @brief Binds various utility functions and easing functions to the Lua scripting environment.
     *
     * This method maps different easing functions and primitive creation
     * functions to corresponding Lua script functions.
     * Additionally, it binds transformation, rotation, scaling, color setting, and
     * entity lifecycle management functions
     * to allow manipulation of entities within the Lua scripts.
     */
    void BindFunctions() {
      // Easing functions
      _lua["EaseInSine"] = EasingFunctions::easeInSine;
      _lua["EaseOutSine"] = EasingFunctions::easeOutSine;
      _lua["EaseInOutSine"] = EasingFunctions::easeInOutSine;
      _lua["EaseInQuad"] = EasingFunctions::easeInQuad;
      _lua["EaseOutQuad"] = EasingFunctions::easeOutQuad;
      _lua["EaseInOutQuad"] = EasingFunctions::easeInOutQuad;
      _lua["EaseInCubic"] = EasingFunctions::easeInCubic;
      _lua["EaseOutCubic"] = EasingFunctions::easeOutCubic;
      _lua["EaseInOutCubic"] = EasingFunctions::easeInOutCubic;
      _lua["EaseInQuart"] = EasingFunctions::easeInQuart;
      _lua["EaseOutQuart"] = EasingFunctions::easeOutQuart;
      _lua["EaseInOutQuart"] = EasingFunctions::easeInOutQuart;
      _lua["EaseInQuint"] = EasingFunctions::easeInQuint;
      _lua["EaseOutQuint"] = EasingFunctions::easeOutQuint;
      _lua["EaseInOutQuint"] = EasingFunctions::easeInOutQuint;
      _lua["EaseInExpo"] = EasingFunctions::easeInExpo;
      _lua["EaseOutExpo"] = EasingFunctions::easeOutExpo;
      _lua["EaseInOutExpo"] = EasingFunctions::easeInOutExpo;
      _lua["EaseInCirc"] = EasingFunctions::easeInCirc;
      _lua["EaseOutCirc"] = EasingFunctions::easeOutCirc;
      _lua["EaseInOutCirc"] = EasingFunctions::easeInOutCirc;
      _lua["EaseInBack"] = EasingFunctions::easeInBack;
      _lua["EaseOutBack"] = EasingFunctions::easeOutBack;
      _lua["EaseInOutBack"] = EasingFunctions::easeInOutBack;
      _lua["EaseInElastic"] = EasingFunctions::easeInElastic;
      _lua["EaseOutElastic"] = EasingFunctions::easeOutElastic;
      _lua["EaseInOutElastic"] = EasingFunctions::easeInOutElastic;
      _lua["EaseInBounce"] = EasingFunctions::easeInBounce;
      _lua["EaseOutBounce"] = EasingFunctions::easeOutBounce;
      _lua["EaseInOutBounce"] = EasingFunctions::easeInOutBounce;

      // Primitives
      _lua["CreateSprite"] = [](const std::string &path) {
        return PrimitiveFactory::CreateSprite(path);
      };

      _lua["CreateQuad"] = []() {
        return PrimitiveFactory::CreateQuad();
      };

      // TRS functions
      _lua["Translate"] = [this](const flecs::entity entity, const float x,
                                 const float y, const float z) {
        if (const auto position = entity.get_mut<Position>(); position != nullptr) {
          const float ppm = _cameraManager->currentCamera->PPM;
          position->pos = glm::vec3(x, y, z);
        }
      };

      _lua["Rotate"] = [](const flecs::entity entity, const float x,
                          const float y, const float z, const float angle) {
        if (const auto rotation = entity.get_mut<Rotation>(); rotation != nullptr) {
          rotation->orientation = glm::vec3(x, y, z);
          rotation->angle = angle;
        }
      };

      _lua["Scale"] = [this](const flecs::entity entity, const float x,
                             const float y, const float z) {
        if (const auto scale = entity.get_mut<Scale>(); scale != nullptr) {
          const float ppm = _cameraManager->currentCamera->PPM;
          scale->scale = glm::vec3(x, y, z);
        }
      };

      // Colors

      _lua["SetColor"] = [](const flecs::entity entity, const float r,
                            const float g, const float b, const float a) {
        if (const auto color = entity.get_mut<Color>(); color != nullptr) {
          color->color = glm::vec4(r, g, b, a);
        }
      };

      // Entity lifecycle
      _lua["Delete"] = [](const flecs::entity entity) {
        if (entity.is_alive()) {
          entity.destruct();
        }
      };

      // 2D physics
      _lua["Create2DDynamicBody"] = [this](const float x, const float y, const float w, const float h) {
        return _physics2d->CreateDynamicBody(x, y, w, h);
      };

      _lua["Get2DDynamicBodyPosition"] = [](const b2BodyId bodyId) {
        return b2Body_GetPosition(bodyId).y;
      };

      // Camera
      _lua["SetCameraPosition"] = [this](const float x, const float y) {
        const auto camera = dynamic_cast<Cameras::OrthographicCamera *>(_cameraManager->currentCamera);
        camera->PanCamera(x, y);
      };
      _lua["SetCameraZoom"] = [this](const float z) {
        const auto camera = dynamic_cast<Cameras::OrthographicCamera *>(_cameraManager->currentCamera);
        camera->Zoom(z);
      };
    }

    sol::state *Get() override { return &_lua; }

  private:
    // Lua state
    sol::state _lua;

    // Dependencies
    std::shared_ptr<IWorld> _world;
    std::shared_ptr<IPhysics2D> _physics2d;
    std::shared_ptr<ICameraManager> _cameraManager;
  };
}
