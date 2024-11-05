#pragma once

#include <servicelocators/servicelocator.hpp>

#include <box2d/box2d.h>

class IPhysics2D : public IService {
public:
  virtual ~IPhysics2D() = default;
  virtual b2WorldId Get() = 0;
  virtual b2BodyId CreateGround(float x, float y, float w, float h) = 0;
  virtual b2BodyId CreateDynamicBody(float x, float y, float w, float h) = 0;
  virtual b2BodyId CreateSensorBody(float x, float y, float w, float h,
                                    b2Vec2 *pos) = 0;
};
