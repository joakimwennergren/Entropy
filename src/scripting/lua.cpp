#include "lua.hpp"

using namespace Entropy::Scripting;

class Awaitable
{
public:
    Entropy::GLTF::Model *Get() { return fut.get(); };
    std::future<Entropy::GLTF::Model *> fut;
};

Lua::Lua(std::shared_ptr<ServiceLocator> serviceLocator)
{
    // Get required depenencies
    auto world = serviceLocator->GetService<World>();

    // auto mouse = std::dynamic_pointer_cast<Mouse>(serviceLocator->getService("Mouse"));
    _lua.set_panic(sol::c_call<decltype(&Lua::my_panic), &Lua::my_panic>);
    _lua.open_libraries(sol::lib::base, sol::lib::coroutine);

    // _lua.new_usertype<Sprite>(
    //     "Sprite",
    //     sol::factories([serviceLocator, this](const std::string path)
    //                    {
    //                        // auto sprite = std::make_shared<Sprite>(serviceLocator, path);
    //                        // sprite->script->environment = sol::environment(_lua, sol::create, _lua.globals());
    //                        // sceneGraph->renderables.push_back(sprite);
    //                        // return sprite;
    //                    }),
    //     "Translate", &Sprite::Translate2D, "Scale", &Sprite::Scale2D, "SetScript", &Sprite::SetScript, "SetScriptFile", &Sprite::SetScriptFile, "Parent", sol::property(&Sprite::parent));

    _lua.new_usertype<Awaitable>(
        "Model",
        sol::factories([serviceLocator, this](const std::string path)
                       {
        if (path.length() > 0)
        {
            auto awaitable = new Awaitable();
            awaitable->fut = std::async(std::launch::async, [serviceLocator, path]
                                   { 
                                                        auto model = new Entropy::GLTF::Model(serviceLocator);
                                                        model->loadFromFile(path);
                                                        return model; });

            return awaitable;
        } }));

    _lua["Translate"] = [](flecs::entity *entity, float x, float y, float z)
    {
        if (entity != nullptr)
        {
            auto pos = entity->get_mut<Entropy::Components::Position>();
            pos->pos = glm::vec3(x, y, z);
        }
    };

    _lua["Scale"] = [](flecs::entity *entity, float x, float y, float z)
    {
        auto pos = entity->get_mut<Entropy::Components::Scale>();
        pos->scale = glm::vec3(x, y, z);
    };

    _lua["AsyncGet"] = [this, serviceLocator](Awaitable *awaitable)
    {
        auto e = serviceLocator->GetService<World>()->gameWorld.entity();

        if (awaitable->fut.valid())
        {
            if (awaitable->fut.wait_for(1ms) == std::future_status::ready)
            {
                auto model = awaitable->Get();

                auto id = AssetId().GetId();
                e.set<Position>({glm::vec3(0.0, 0.0, 0.0)});
                e.set<Scale>({glm::vec3(1.0, 1.0, 1.0)});
                e.set<Rotation>({glm::vec3(1.0, 1.0, 1.0), 1.0});
                e.set<Entropy::Components::Model>({model});
                e.set<Entropy::Components::Renderable>({id, 0, true});
                e.set<Entropy::Components::Color>({glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}});
                e.set<Entropy::Components::BoxCollisionShape3D>({glm::vec3(10.0, 10.0, 10.0), glm::vec3{0.0, 0.0, 0.0}});
                e.set<Entropy::Components::RigidBody3D>({});
                e.set<Entropy::Components::Scripted>({});
                e.get_mut<Entropy::Components::BoxCollisionShape3D>()->UpdateMotionState();
                e.get_mut<Entropy::Components::RigidBody3D>()->Initialize(serviceLocator, e);
                return e;
            }
        }

        return e;
    };

    _lua["LoadModelAsync"] = [this, serviceLocator](std::string path) {

    };
}
