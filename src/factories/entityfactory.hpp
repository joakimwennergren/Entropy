#pragma once

#include "ecs/components/objmodel.hpp"
#include "factories/vulkan/bufferfactory.hpp"
#include "graphics/vulkan/vulkan_backend.hpp"
#include "obj/model.hpp"
#include <scripting/lua.hpp>

namespace Entropy {
namespace Factories {

struct EntityFactory {

  EntityFactory(Lua lua, World world, VulkanBackend vbe, BufferFactory bf)
      : _lua{lua}, _world{world}, _backend{vbe}, _bufferFactory{bf} {}

  flecs::entity CreateOBJModel(std::string filePath) {
    auto model = std::make_shared<OBJ::ObjModel>(_backend, _bufferFactory);
    model->loadFromFile("/Users/joakim/Desktop/GTKPoC/12140_Skull_v3_L2.obj",
                        "");
    auto e = _world.gameWorld->entity();
    auto id = AssetId().GetId();
    e.set<Position>({glm::vec3(0.0, 0.0, 0.0)});
    e.set<Scale>({glm::vec3(1.0, 1.0, 1.0)});
    e.set<Rotation>({glm::vec3(1.0, 1.0, 1.0), 1.0});
    e.set<Entropy::Components::OBJModel>({model});
    e.set<Entropy::Components::Renderable>({id, 0, true});
    e.set<Entropy::Components::Color>({glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}});
    return e;
  }

private:
  Lua _lua;
  World _world;
  VulkanBackend _backend;
  BufferFactory _bufferFactory;
};

} // namespace Factories
} // namespace Entropy