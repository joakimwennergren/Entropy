#pragma once

#include <box2d/box2d.h>
#include <vulkan/vulkan.hpp>

#include <iostream>

#include "box2d/b2_settings.h"
#include "box2d/b2_world.h"
#include "factories/entityfactory.hpp"
#include "spdlog/spdlog.h"
#include <glm/glm.hpp>

class b2DebugDraw : public b2Draw {
public:
  b2DebugDraw(Entropy::Factories::EntityFactory factory,
              std::vector<flecs::entity> &entities)
      : _entityFactory{factory} {
    _entityFactory = factory;
    std::vector<Vertex> _vertices(4);
    _entityFactory.CreateQuad(_vertices);
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

    entites.push_back(_entityFactory.CreateQuad(_vertices));
  }

  void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount,
                        const b2Color &color){};

  void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color){};

  void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis,
                       const b2Color &color){};

  void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color){};

  void DrawTransform(const b2Transform &xf){};

  void DrawPoint(const b2Vec2 &p, float size, const b2Color &color){};

  void DrawString(int x, int y, const char *string, ...){};

  void DrawAABB(b2AABB *aabb, const b2Color &color){};

  Entropy::Factories::EntityFactory _entityFactory;

  std::vector<flecs::entity> entites;
};

namespace Entropy {
namespace Physics {

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

class Physics2D {

public:
  const float PPM = 100.0f;
  std::vector<flecs::entity> _entities;
  Physics2D(Factories::EntityFactory entityFactory)
      : _entityFactory{entityFactory} {

    _debugDraw = new b2DebugDraw(_entityFactory, _entities);
    b2Vec2 gravity(0.0f, -5.0f);
    world = new b2World(gravity);
    // world->SetDebugDraw(_debugDraw);
    // uint32 flags = 0;
    // flags += b2Draw::e_shapeBit;
    // flags += b2Draw::e_jointBit;
    // flags += b2Draw::e_aabbBit;
    // flags += b2Draw::e_pairBit;
    // flags += b2Draw::e_centerOfMassBit;
    // _debugDraw->SetFlags(flags);
    auto listener = new MyContactListener();
    world->SetContactListener(listener);
  }

  inline b2Body *CreateGround(float x, float y, float w, float h) {
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(x, y);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(w / PPM, h / PPM);
    b2Body *groundBody = world->CreateBody(&groundBodyDef);
    groundBody->CreateFixture(&groundBox, 0.0f);
    return groundBody;
  }

  inline b2Body *CreateDynamicBody(float x, float y, float w, float h) {

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);

    b2Body *body = world->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(w / PPM, h / PPM);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.1f;
    fixtureDef.friction = 1.0f;
    fixtureDef.restitution = 0.0f;
    body->CreateFixture(&fixtureDef);

    return body;
  }

  inline b2Body *CreateSensorBody(float x, float y, float w, float h,
                                  b2Vec2 *pos) {

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
  }

  b2World *world;
  b2DebugDraw *_debugDraw;
  Factories::EntityFactory _entityFactory;

private:
};

class DynamicBody {
public:
  DynamicBody(std::shared_ptr<Physics2D> physics){
      // body = physics->CreateDynamicBody();
  };

  float GetPosition() {
    // auto pos = body->GetPosition();
    // return pos.y;
    return 0.1;
  }

  // b2Body *body;
};
} // namespace Physics
} // namespace Entropy
