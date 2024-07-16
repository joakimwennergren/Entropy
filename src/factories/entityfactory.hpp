#pragma once

#include "ecs/components/hasTexture.hpp"
#include "ecs/components/objmodel.hpp"
#include "factories/vulkan/bufferfactory.hpp"
#include "factories/vulkan/texturefactory.hpp"
#include "filesystem/filesystem.hpp"
#include "graphics/vulkan/vulkan_backend.hpp"
#include "obj/model.hpp"
#include <assets/assetid.hpp>
#include <ecs/world.hpp>
#include <graphics/primitives/2d/sprite.hpp>

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
#include <ecs/components/sprite.hpp>
#include <ecs/components/tags/scripted.hpp>
#include <ecs/components/trianglemeshcollisionshape3d.hpp>

using namespace Entropy::Components;
using namespace Entropy::ECS;

namespace Entropy {
namespace Factories {

struct EntityFactory {

  EntityFactory(World world, VulkanBackend vbe, BufferFactory bf,
                TextureFactory tf)
      : _world{world}, _backend{vbe}, _bufferFactory{bf}, _textureFactory{tf} {}

  flecs::entity CreateOBJModel(std::string filePath) {
    auto model = std::make_shared<OBJ::ObjModel>(_backend, _bufferFactory,
                                                 _textureFactory);
    model->loadFromFile(
        "/Users/joakim/Desktop/Entropy-Editor/12140_Skull_v3_L2.obj",
        "/Users/joakim/Desktop/models/Skull.png");
    auto e = _world.gameWorld->entity();
    auto id = AssetId().GetId();
    e.set<Position>({glm::vec3(0.0, 0.0, 0.0)});
    e.set<Scale>({glm::vec3(25.0, 25.0, 25.0)});
    e.set<Rotation>({glm::vec3(1.0, 1.0, 1.0), 0.0});
    e.set<Entropy::Components::OBJModel>({model});
    auto renderable = Entropy::Components::Renderable();
    renderable.id = id;
    renderable.vertexBuffer = model->vertexBuffer;
    renderable.vertices = model->vertices;
    e.set(renderable);
    e.set<Entropy::Components::Color>({glm::vec4{1.0f, 0.0f, 1.0f, 1.0f}});
    e.set<Entropy::Components::HasTexture>({model->texture});
    return e;
  }

  flecs::entity CreateSprite(std::string path) {
    auto sprite = std::make_shared<Entropy::Graphics::Primitives::Sprite>(
        _bufferFactory, _textureFactory, GetSpritesDir() + path);
    auto e = _world.gameWorld->entity();
    auto id = AssetId().GetId();
    e.set<Position>({glm::vec3(0.0, 0.0, 0.0)});
    e.set<Scale>({glm::vec3(25.0, 25.0, 25.0)});
    e.set<Rotation>({glm::vec3(1.0, 1.0, 1.0), 0.0});
    e.set<Entropy::Components::SpriteComponent>({sprite});
    e.set<Entropy::Components::Renderable>(
        {id, true, sprite->vertexBuffer, sprite->indexBuffer, sprite->indices});
    e.set<Entropy::Components::Color>({glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}});
    e.set<Entropy::Components::HasTexture>({sprite->texture});
    return e;
  }

private:
  World _world;
  VulkanBackend _backend;
  BufferFactory _bufferFactory;
  TextureFactory _textureFactory;
};

} // namespace Factories
} // namespace Entropy