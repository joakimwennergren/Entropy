#pragma once

#include <box2d/box2d.h>
#include <cameras/icamera_manger.hpp>

#include "iphysics2d.hpp"

namespace Entropy::Physics {
  struct Physics2D final : ServiceBase<IPhysics2D> {
    Physics2D() {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      _cameraManager = sl->getService<ICameraManager>();
      b2WorldDef worldDef = b2DefaultWorldDef();
      worldDef.gravity = (b2Vec2){0.0f, -10.0f};
      _world = b2CreateWorld(&worldDef);
    }

    b2WorldId Get() override { return _world; }

    b2BodyId CreateKinematicBody(const float x, const float y, const float w, const float h) override {
      const float ppm = _cameraManager->currentCamera->PPM;
      b2BodyDef bodyDef = b2DefaultBodyDef();
      bodyDef.position = b2Vec2{x, y};
      bodyDef.type = b2_kinematicBody;
      const b2Polygon groundBox = b2MakeBox(w / ppm, h / ppm);
      const b2BodyId body = b2CreateBody(_world, &bodyDef);
      const b2ShapeDef groundShapeDef = b2DefaultShapeDef();
      b2CreatePolygonShape(body, &groundShapeDef, &groundBox);
      return body;
    }

    b2BodyId CreateDynamicBody(const float x, const float y, const float w, const float h) override {
      const float ppm = _cameraManager->currentCamera->PPM;
      b2BodyDef bodyDef = b2DefaultBodyDef();
      bodyDef.position = b2Vec2{x, y};
      bodyDef.type = b2_dynamicBody;
      const b2Polygon groundBox = b2MakeBox(w / ppm, h / ppm);
      const b2BodyId body = b2CreateBody(_world, &bodyDef);
      const b2ShapeDef groundShapeDef = b2DefaultShapeDef();
      b2CreatePolygonShape(body, &groundShapeDef, &groundBox);
      return body;
    }

  private:
    b2WorldId _world{};
    std::shared_ptr<ICameraManager> _cameraManager;
  };
} // namespace Entropy::Physics
