#pragma once

#include <graphics/primitives/2d/sprite.hpp>

#include <ecs/components/position.hpp>
#include <ecs/components/renderable.hpp>
#include <ecs/components/rotation.hpp>
#include <ecs/components/scale.hpp>
#include <ecs/components/sprite.hpp>

#include <flecs.h>
#include <assets/assetid.hpp>
#include <filesystem/filesystem.hpp>
#include <ecs/iworld.hpp>
#include <ecs/components/color.hpp>
#include <ecs/components/hasTexture.hpp>

using namespace Entropy::Components;
using namespace Entropy::Graphics::Primitives;
using namespace Entropy::ECS;
using namespace Entropy::Filesystem;

namespace Entropy::Graphics::Primitives {
    struct PrimitiveFactory {
        static flecs::entity CreateSprite(const std::string &path) {
            const ServiceLocator *sl = ServiceLocator::GetInstance();
            const auto world = sl->getService<IWorld>();
            const auto sprite = std::make_shared<Sprite>(GetSpritesDir() + path);
            const auto e = world->Get()->entity();
            const auto id = AssetId().GetId();
            e.set<Position>({glm::vec3(0.0, 0.0, 0.0)});
            e.set<Scale>({glm::vec3(1.0, 1.0, 1.0)});
            e.set<Rotation>({glm::vec3(1.0, 1.0, 1.0), 0.0});
            e.set<SpriteComponent>({sprite});
            auto renderable = Renderable();
            renderable.visible = true;
            renderable.id = id;
            renderable.indexBuffer = sprite->indexBuffer;
            renderable.vertexBuffer = sprite->vertexBuffer;
            renderable.indices = sprite->indices;
            renderable.type = 2;
            e.set<Renderable>(renderable);
            e.set<Color>({glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}});
            e.set<HasTexture>({sprite->texture});
            return e;
        }
    };
}
