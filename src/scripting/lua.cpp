#include "lua.hpp"

using namespace Entropy::Scripting;

Lua::Lua(std::shared_ptr<ServiceLocator> serviceLocator)
{
    // Get required depenencies
    auto world = serviceLocator->GetService<World>();

    // auto mouse = std::dynamic_pointer_cast<Mouse>(serviceLocator->getService("Mouse"));
    _lua.set_panic(sol::c_call<decltype(&Lua::my_panic), &Lua::my_panic>);
    _lua.open_libraries(sol::lib::base, sol::lib::coroutine);

    _lua.new_usertype<Sprite>(
        "Sprite",
        sol::factories([serviceLocator, this](const std::string path)
                       {
                           // auto sprite = std::make_shared<Sprite>(serviceLocator, path);
                           // sprite->script->environment = sol::environment(_lua, sol::create, _lua.globals());
                           // sceneGraph->renderables.push_back(sprite);
                           // return sprite;
                       }),
        "Translate", &Sprite::Translate2D, "Scale", &Sprite::Scale2D, "SetScript", &Sprite::SetScript, "SetScriptFile", &Sprite::SetScriptFile, "Parent", sol::property(&Sprite::parent));

    _lua.new_usertype<Entropy::GLTF::Model>(
        "Model",
        sol::factories([serviceLocator, this](const std::string path)
                       {
                           if (path.length() > 0)
                           {
 
                           } }));

    _lua["Translate"] = [](flecs::entity entity, float x, float y, float z)
    {
        if (entity.is_alive())
        {
            auto pos = entity.get_mut<Entropy::Components::Position>();
            pos->pos = glm::vec3(x, y, z);
        }
    };

    _lua["DONT_RELOAD"] = [] {

    };

    _lua["STOP_DONT_RELOAD"] = [] {

    };

    _lua["Scale"] = [](flecs::entity *entity, float x, float y, float z)
    {
        auto pos = entity->get_mut<Entropy::Components::Scale>();
        pos->scale = glm::vec3(x, y, z);
    };

    _lua["Get"] = [this](int id)
    {
        return loadedModels[id];
    };

    _lua["LoadModelAsync"] = [this, serviceLocator](std::string path)
    {
        futures.push_back(std::async(std::launch::async, [serviceLocator, path]
                                     { 
                                auto model = new Entropy::GLTF::Model(serviceLocator);
                                model->loadFromFile(path);
                                return model; }));
        return futures.size() - 1;
    };
}
