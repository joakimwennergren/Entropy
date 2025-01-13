#pragma once

#include <graphics/primitives/2d/sprite.hpp>
#include <graphics/primitives/2d/quad.hpp>

#include <ecs/components/position.hpp>
#include <ecs/components/renderable.hpp>
#include <ecs/components/rotation.hpp>
#include <ecs/components/scale.hpp>
#include <ecs/components/sprite.hpp>
#include <ecs/components/quad.hpp>

#include <flecs.h>
#include <assets/assetid.hpp>
#include <filesystem/filesystem.hpp>
#include <ecs/iworld.hpp>
#include <ecs/components/color.hpp>
#include <ecs/components/hasTexture.hpp>

#include "ecs/components/tags/dynamicbody.h"

using namespace Entropy::Components;
using namespace Entropy::Graphics::Primitives;
using namespace Entropy::ECS;
using namespace Entropy::Filesystem;

namespace Entropy::Graphics::Primitives {
    struct PrimitiveFactory {
        /**
         * Create a new sprite entity from the given sprite path.
         *
         * @param path The relative path to the sprite file.
         * @return A flecs::entity representing the created sprite entity.
         *
         * This function constructs a sprite entity with various components such as Position,
         * Scale, Rotation, SpriteComponent, Renderable, Color, and HasTexture. It relies on
         * the ServiceLocator to get an instance of the IWorld service and assigns initial values
         * to these components.
         */
        static flecs::entity CreateSprite(const std::string &path) {
            const ServiceLocator *sl = ServiceLocator::GetInstance();
            const auto world = sl->getService<IWorld>();
            const auto sprite = std::make_shared<Sprite>(path);
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

        /**
         * Create a new quad entity.
         *
         * @return A flecs::entity representing the created quad entity.
         *
         * This function constructs a quad entity with predefined components,
         * including Position, Scale, Rotation, SpriteComponent, Renderable, Color,
         * and HasTexture. It utilizes the ServiceLocator to obtain an instance
         * of the IWorld service, and assigns various components to the entity
         * such as position, scale, rotation, and rendering details.
         */
        static flecs::entity CreateQuad(const int type) {
            const ServiceLocator *sl = ServiceLocator::GetInstance();
            const auto world = sl->getService<IWorld>();
            const auto quad = std::make_shared<Quad>();
            const auto e = world->Get()->entity();
            const auto id = AssetId().GetId();
            e.set<Position>({glm::vec3(0.0, 0.0, 0.0)});
            e.set<Scale>({glm::vec3(1.0, 1.0, 1.0)});
            e.set<Rotation>({glm::vec3(1.0, 1.0, 1.0), 0.0});
            e.set<QuadComponent>({quad});
            auto renderable = Renderable();
            renderable.visible = true;
            renderable.id = id;
            renderable.indexBuffer = quad->indexBuffer;
            renderable.vertexBuffer = quad->vertexBuffer;
            renderable.indices = quad->indices;
            renderable.type = type;
            e.set<Renderable>(renderable);
            e.set<Color>({glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}});
            e.set<HasTexture>({quad->texture});
            return e;
        }

        static flecs::entity CreateQuadFromVertices(const std::vector<Vertex> &vertices) {
            const ServiceLocator *sl = ServiceLocator::GetInstance();
            const auto world = sl->getService<IWorld>();
            const auto quad = std::make_shared<Quad>(vertices);
            const auto e = world->Get()->entity();
            const auto id = AssetId().GetId();
            e.set<Position>({glm::vec3(0.0, 0.0, 0.0)});
            e.set<Scale>({glm::vec3(1.0, 1.0, 1.0)});
            e.set<Rotation>({glm::vec3(1.0, 1.0, 1.0), 0.0});
            e.set<QuadComponent>({quad});
            e.set<DynamicBodyComponent>({true});
            auto renderable = Renderable();
            renderable.visible = true;
            renderable.id = id;
            renderable.indexBuffer = quad->indexBuffer;
            renderable.vertexBuffer = quad->vertexBuffer;
            renderable.indices = quad->indices;
            renderable.type = 2;
            e.set<Renderable>(renderable);
            e.set<Color>({glm::vec4{1.0f, 0.0f, 0.0f, 1.0f}});
            e.set<HasTexture>({quad->texture});
            return e;
        }
    };
}
