#include "lua.hpp"

using namespace Entropy::Scripting;

Lua::Lua(std::shared_ptr<ServiceLocator> serviceLocator)
{
    // Get required depenencies
    auto sceneGraph = serviceLocator->GetService<SceneGraph>();
    auto physics2d = serviceLocator->GetService<Physics2D>();
    // auto mouse = std::dynamic_pointer_cast<Mouse>(serviceLocator->getService("Mouse"));

    lua.open_libraries(sol::lib::base);

    lua.new_usertype<DynamicBody>("DynamicBody",
                                  sol::factories([physics2d]()
                                                 { auto dynBody = std::make_shared<DynamicBody>(physics2d);
                                                   return dynBody; }),
                                  "GetPosition", &DynamicBody::GetPosition);

    lua.new_usertype<Sprite>(
        "Sprite",
        sol::factories([serviceLocator, sceneGraph, this](const std::string path)
                       {
                                        auto sprite = std::make_shared<Sprite>(serviceLocator, path);
                                        sprite->script->environment = sol::environment(lua, sol::create, lua.globals());
                                        sceneGraph->renderables.push_back(sprite);
                                        return sprite; }),
        "Translate", &Sprite::Translate2D,
        "Scale", &Sprite::Scale2D,
        "SetScript", &Sprite::SetScript,
        "SetScriptFile", &Sprite::SetScriptFile,
        "Parent", sol::property(&Sprite::parent));

    // lua.script_file("/Users/joakim/Desktop/Entropy-Engine/resources/scripts/leaf.lua");
    // lua.script_file("/Users/joakim/Desktop/Entropy-Engine/resources/scripts/main.lua");
}
