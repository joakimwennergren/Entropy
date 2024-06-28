#include "lua.hpp"

using namespace Entropy::Scripting;

flecs::entity Lua::GetAsync(std::shared_ptr<Entropy::GLTF::Model> model) {
  auto e = _serviceLocator->GetService<World>()->gameWorld.entity();

  auto id = AssetId().GetId();
  e.set<Position>({glm::vec3(0.0, 0.0, 0.0)});
  e.set<Scale>({glm::vec3(1.0, 1.0, 1.0)});
  e.set<Rotation>({glm::vec3(1.0, 1.0, 1.0), 1.0});
  e.set<Entropy::Components::Model>({model});
  e.set<Entropy::Components::Renderable>({id, 0, true});
  e.set<Entropy::Components::Color>({glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}});
  model->getSceneDimensions();
  e.set<Entropy::Components::BoxCollisionShape3D>(
      {glm::vec3{model->dimensions.max.x, model->dimensions.max.y / 2.0,
                 model->dimensions.max.z},
       glm::vec3{0.0, 0.0 - (model->dimensions.max.y / 2.0), 0.0}});
  e.set<Entropy::Components::RigidBody3D>({});
  e.set<Entropy::Components::Scripted>({});
  e.get_mut<Entropy::Components::BoxCollisionShape3D>()->UpdateMotionState();
  e.get_mut<Entropy::Components::RigidBody3D>()->Initialize(_serviceLocator, e);
  return e;
}

Lua::Lua(std::shared_ptr<ServiceLocator> serviceLocator) {
  _serviceLocator = serviceLocator;
  // Get required depenencies
  auto world = serviceLocator->GetService<World>();

  _world = world;

  // auto mouse =
  // std::dynamic_pointer_cast<Mouse>(serviceLocator->getService("Mouse"));
  _lua.open_libraries(sol::lib::base, sol::lib::coroutine);

  // _lua.new_usertype<Sprite>(
  //     "Sprite",
  //     sol::factories([serviceLocator, this](const std::string path)
  //                    {
  //                        // auto sprite =
  //                        std::make_shared<Sprite>(serviceLocator, path);
  //                        // sprite->script->environment =
  //                        sol::environment(_lua, sol::create, _lua.globals());
  //                        // sceneGraph->renderables.push_back(sprite);
  //                        // return sprite;
  //                    }),
  //     "Translate", &Sprite::Translate2D, "Scale", &Sprite::Scale2D,
  //     "SetScript", &Sprite::SetScript, "SetScriptFile",
  //     &Sprite::SetScriptFile, "Parent", sol::property(&Sprite::parent));

  _lua.new_usertype<Entropy::GLTF::Model>(
      "Model", sol::factories([serviceLocator, this](const std::string path) {
        if (path.length() > 0) {

          auto model = std::make_shared<Entropy::GLTF::Model>(serviceLocator);
          model->loadFromFile(path);
          auto e = _serviceLocator->GetService<World>()->gameWorld.entity();

          auto id = AssetId().GetId();
          e.set<Position>({glm::vec3(0.0, 0.0, 0.0)});
          e.set<Scale>({glm::vec3(1.0, 1.0, 1.0)});
          e.set<Rotation>({glm::vec3(1.0, 1.0, 1.0), 1.0});
          e.set<Entropy::Components::Model>({model});
          e.set<Entropy::Components::Renderable>({id, 0, true});
          e.set<Entropy::Components::Color>(
              {glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}});
          model->getSceneDimensions();
          e.set<Entropy::Components::BoxCollisionShape3D>(
              {glm::vec3{model->dimensions.max.x, model->dimensions.max.y / 2.0,
                         model->dimensions.max.z},
               glm::vec3{0.0, 0.0 - (model->dimensions.max.y / 2.0), 0.0}});
          e.set<Entropy::Components::RigidBody3D>({});
          e.set<Entropy::Components::Scripted>({});
          e.get_mut<Entropy::Components::BoxCollisionShape3D>()
              ->UpdateMotionState();
          e.get_mut<Entropy::Components::RigidBody3D>()->Initialize(
              _serviceLocator, e);

          spdlog::info("Returning entity!");
          return e;

          // auto awaitable = new Awaitable();
          // awaitable->fut = std::async(std::launch::async, [serviceLocator,
          // path]
          //                        {
          //                                             auto model =
          //                                             std::make_shared<Entropy::GLTF::Model>(serviceLocator);
          //                                             model->loadFromFile(path);
          //                                             return model; });

          // return awaitable;
        }
      }));

  _lua["Translate"] = [this](flecs::entity entity, float x, float y, float z) {
    if (!entity.is_alive())
      return;

    auto pos = entity.get_mut<Entropy::Components::Position>();
    pos->pos = glm::vec3(x, y, z);
  };

  _lua["Scale"] = [this](flecs::entity entity, float x, float y, float z) {
    if (!entity.is_alive())
      return;

    auto pos = entity.get_mut<Entropy::Components::Scale>();
    pos->scale = glm::vec3(x, y, z);
  };

  _lua["Rotate"] = [this](flecs::entity entity, float angle) {
    if (!entity.is_alive())
      return;

    auto rot = entity.get_mut<Rotation>();
    rot->angle = angle;
    rot->orientation = glm::vec3(0.0, 1.0, 0.0);
  };

  _lua["RunAnimation"] = [](std::optional<flecs::entity> optional, float tick) {
    if (!optional.has_value())
      return;

    auto entity = optional.value();

    if (!entity.is_alive())
      return;

    auto model = entity.get_ref<Entropy::Components::Model>();
    model->model->updateAnimation(0, tick);
  };

  _lua["Destroy"] = [serviceLocator](flecs::entity e) {
    serviceLocator->GetService<QueueSync>()->_deletables.push_back(e);
  };

  _lua["AsyncGet"] = [this](std::shared_ptr<Entropy::GLTF::Model> m) {
    return GetAsync(m);
  };
}
