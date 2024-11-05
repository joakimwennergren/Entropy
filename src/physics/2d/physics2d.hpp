#pragma once

#include <box2d/box2d.h>
#include <vulkan/vulkan.hpp>

#include <iostream>

#include "iphysics2d.hpp"

#include <data/vertex.hpp>
#include <ecs/components/color.hpp>
#include <ecs/components/hasTexture.hpp>
#include <ecs/components/position.hpp>
#include <ecs/components/quad.hpp>
#include <ecs/components/renderable.hpp>
#include <ecs/components/rigidbody3d.hpp>
#include <ecs/components/rotation.hpp>
#include <ecs/components/scale.hpp>
#include <ecs/iworld.hpp>
#include <flecs.h>
#include <glm/glm.hpp>
#include <graphics/primitives/2d/quad.hpp>
#include <spdlog/spdlog.h>

#include <assets/assetid.hpp>

using namespace Entropy::Data;
using namespace Entropy::ECS;

/*

class b2DebugDraw : public b2Draw {
public:
  b2DebugDraw(std::vector<flecs::entity> &entities) {
    ServiceLocator *sl = ServiceLocator::GetInstance();
    _entityWorld = sl->getService<IWorld>();
    // std::vector<Vertex> _vertices(4);
    // _entityFactory.CreateQuad(_vertices);
  }

  void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount,
                   const b2Color &color) {
    std::vector<Vertex> _vertices;

    for (int i = 0; i < vertexCount; i++) {
      Vertex newVert;
      newVert.position.x = vertices[i].x;
      newVert.position.y = vertices[i].y;
      newVert.color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
      _vertices.push_back(newVert);
    }

    auto quad =
        std::make_shared<Entropy::Graphics::Primitives::Quad>(_vertices);
    auto e = _entityWorld->Get()->entity();
    auto id = AssetId().GetId();
    e.set<Entropy::Components::Position>({glm::vec3(0.0, 0.0, 0.0)});
    e.set<Entropy::Components::Scale>({glm::vec3(1.0, 1.0, 1.0)});
    e.set<Entropy::Components::Rotation>({glm::vec3(1.0, 1.0, 1.0), 0.0});
    e.set<Entropy::Components::QuadComponent>({quad});
    e.set<Entropy::Components::Renderable>(
        {id, true, quad->vertexBuffer, quad->indexBuffer, quad->indices});
    e.set<Entropy::Components::Color>({glm::vec4{1.0f, 0.0f, 0.0f, 1.0f}});
    e.set<Entropy::Components::HasTexture>({quad->texture});
    entites.push_back(e);
  }

  void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount,
                        const b2Color &color) {};

  void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) {};

  void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis,
                       const b2Color &color) {};

  void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {};

  void DrawTransform(const b2Transform &xf) {};

  void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) {};

  void DrawString(int x, int y, const char *string, ...) {};

  void DrawAABB(b2AABB *aabb, const b2Color &color) {};

  std::vector<flecs::entity> entites;
  std::shared_ptr<IWorld> _entityWorld;
};

*/

namespace Entropy {
namespace Physics {

/*

class MyContactListener : public b2ContactListener {
public:
void BeginContact(b2Contact *contact) override {
  // Get the two fixtures involved in the collision
  b2Fixture *fixtureA = contact->GetFixtureA();
  b2Fixture *fixtureB = contact->GetFixtureB();

  // Check if either fixture is the sensor
  if (fixtureA->IsSensor() || fixtureB->IsSensor()) {
    // Handle the sensor collision event here

    auto pos = reinterpret_cast<b2Vec2 *>(fixtureA->GetUserData().pointer);
    if (pos != nullptr) {
      fixtureB->GetBody()->GetUserData().pointer =
          reinterpret_cast<uintptr_t>(pos);
      // spdlog::info("Position = {},{}", pos->x, pos->y);
    }

    // Perform any actions needed for the sensor collision
  }
}

void EndContact(b2Contact *contact) override {
  // Handle end of contact if needed
}
};

*/

class Physics2D : public ServiceBase<IPhysics2D> {

public:
  const float PPM = 100.0f;
  std::vector<flecs::entity> _entities;
  Physics2D() {
    ServiceLocator *sl = ServiceLocator::GetInstance();
    _entityWorld = sl->getService<IWorld>();

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, -10.0f};
    //_debugDraw = new b2DebugDraw(_entities);
    world = b2CreateWorld(&worldDef);
    /*
    world->SetDebugDraw(_debugDraw);
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;
    _debugDraw->SetFlags(flags);
    */
    // auto listener = new MyContactListener();
    // world->SetContactListener(listener);
  }

  inline b2WorldId Get() { return world; }

  inline b2BodyId CreateGround(float x, float y, float w, float h) {
    // b2BodyDef groundBodyDef;
    // groundBodyDef.position.Set(x, y);
    // b2PolygonShape groundBox;
    // groundBox.SetAsBox(w / PPM, h / PPM);
    // b2Body *groundBody = world->CreateBody(&groundBodyDef);
    // groundBody->CreateFixture(&groundBox, 0.0f);
    // return groundBody;

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = b2Vec2{x, y};
    bodyDef.type = b2_kinematicBody;
    b2Polygon groundBox = b2MakeBox(w, h);
    b2BodyId body = b2CreateBody(world, &bodyDef);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(body, &groundShapeDef, &groundBox);
    return body;
  }

  inline b2BodyId CreateDynamicBody(float x, float y, float w, float h) {

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = b2Vec2{x, y};
    bodyDef.type = b2_dynamicBody;
    b2Polygon groundBox = b2MakeBox(w, h);
    b2BodyId body = b2CreateBody(world, &bodyDef);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(body, &groundShapeDef, &groundBox);
    return body;
  }

  inline b2BodyId CreateSensorBody(float x, float y, float w, float h,
                                   b2Vec2 *pos) {

    /*

b2BodyDef bodyDef;
bodyDef.type = b2_kinematicBody;
bodyDef.position.Set(x, y);

b2Body *body = world->CreateBody(&bodyDef);
b2PolygonShape dynamicBox;
dynamicBox.SetAsBox(w / PPM, h / PPM);

b2FixtureUserData userData;
userData.pointer = reinterpret_cast<uintptr_t>(pos);

b2FixtureDef fixtureDef;
fixtureDef.shape = &dynamicBox;
fixtureDef.density = 3.0f;
fixtureDef.isSensor = true;
fixtureDef.friction = 0.8f;

fixtureDef.userData = userData;
body->CreateFixture(&fixtureDef);

return body;

*/
  }
  b2WorldId world;
  std::shared_ptr<IWorld> _entityWorld;
  b2DebugDraw *_debugDraw;

private:
};
} // namespace Physics
} // namespace Entropy
