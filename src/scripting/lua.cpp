#include "lua.hpp"

using namespace Entropy::Scripting;

Lua::Lua(std::shared_ptr<ServiceLocator> serviceLocator)
{
    // Get required depenencies
    auto sceneGraph = serviceLocator->GetService<SceneGraph>();
    auto world = serviceLocator->GetService<World>();

    // auto mouse = std::dynamic_pointer_cast<Mouse>(serviceLocator->getService("Mouse"));

    _lua.open_libraries(sol::lib::base);

    _lua.new_usertype<Sprite>(
        "Sprite",
        sol::factories([serviceLocator, sceneGraph, this](const std::string path)
                       {
                                        auto sprite = std::make_shared<Sprite>(serviceLocator, path);
                                        sprite->script->environment = sol::environment(_lua, sol::create, _lua.globals());
                                        sceneGraph->renderables.push_back(sprite);
                                        return sprite; }),
        "Translate", &Sprite::Translate2D,
        "Scale", &Sprite::Scale2D,
        "SetScript", &Sprite::SetScript,
        "SetScriptFile", &Sprite::SetScriptFile,
        "Parent", sol::property(&Sprite::parent));

    _lua.new_usertype<Entropy::GLTF::Model>(
        "Model",
        sol::factories([serviceLocator, this](const std::string path)
                       {
                           auto e = serviceLocator->GetService<World>()->gameWorld.entity();
                           auto assetManager = serviceLocator->GetService<AssetManager>();
                           auto _tex = assetManager->LoadTextureAsync("/Users/joakim/Entropy-Engine/resources/models/Untitled_Scan_19_27_58.gltf");
                           if (_tex.valid())
                           {
                               auto model = std::shared_ptr<Entropy::GLTF::Model>(_tex.get());
                               e.set<Position>({glm::vec3(0.0, 0.0, 0.0)});
                               e.set<Scale>({glm::vec3(100.0, 100.0, 100.0)});
                               e.set<Entropy::Components::Model>({model});
                               e.set<Entropy::Components::Renderable>({2, 0, true});
                               e.set<Entropy::Components::Color>({glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}});
                               e.set<Entropy::Components::BoxCollisionShape3D>({glm::vec3(10.0, 10.0, 10.0), glm::vec3{0.0, 0.0, 0.0}});
                               e.set<Entropy::Components::RigidBody3D>({});
                               e.set<Entropy::Components::Scripted>({});
                            //    e.get_ref<Entropy::Components::BoxCollisionShape3D>()->UpdateMotionState();
                            //    e.get_ref<Entropy::Components::RigidBody3D>()->Initialize(serviceLocator, &e);
                               return e;


                           } }));

    _lua["Translate"] = [](flecs::entity entity, float x, float y, float z)
    {
        auto pos = entity.get_mut<Entropy::Components::Position>();
        pos->pos = glm::vec3(x, y, z);
    };

    // lua.script_file("/Users/joakim/Desktop/Entropy-Engine/resources/scripts/leaf.lua");
    //_lua.script_file("/Users/joakim/Entropy-Engine/resources/scripts/main.lua");
}
