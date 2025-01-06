#pragma once

#include <servicelocators/servicelocator.hpp>

#include <box2d/box2d.h>

struct IPhysics2D : IService {
  ~IPhysics2D() override = default;

  virtual b2WorldId Get() = 0;

  virtual b2BodyId CreateKinematicBody(float x, float y, float w, float h) = 0;

  virtual b2BodyId CreateDynamicBody(float x, float y, float w, float h) = 0;

  std::vector<flecs::entity> debugDrawEntities;
};
