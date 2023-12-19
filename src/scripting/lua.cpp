#include "lua.hpp"

using namespace Entropy::Scripting;

Lua::Lua(std::shared_ptr<ServiceLocator> serviceLocator)
{
    // Get required depenencies
    auto sceneGraph = std::dynamic_pointer_cast<SceneGraph>(serviceLocator->getService("SceneGraph"));
    auto physics2d = std::dynamic_pointer_cast<Physics2D>(serviceLocator->getService("2DPhysics"));

    lua.open_libraries(sol::lib::base);

    lua.new_usertype<DynamicBody>("DynamicBody",
                                  sol::factories([physics2d]()
                                                 { auto dynBody = std::make_shared<DynamicBody>(physics2d);
                                                   
                                                   return dynBody; }),
                                  "GetPosition", &DynamicBody::GetPosition);

    lua.new_usertype<Sprite>("Sprite",
                             sol::factories([serviceLocator, sceneGraph](const std::string path)
                                            {
                                                auto sprite = std::make_shared<Sprite>(serviceLocator, path);
                                                sceneGraph->renderables.push_back(sprite);
                                                return sprite; }),
                             "Translate", &Sprite::Translate2D, "Scale", &Sprite::Scale2D);

    lua.set_function("onRender", [](sol::function cb)
                     { cb(NULL, "THE USER"); });

    lua.script_file("/Users/joakim/Desktop/Entropy-Engine/resources/scripts/leaf.lua");
    lua.script_file("/Users/joakim/Desktop/Entropy-Engine/resources/scripts/main.lua");
}
