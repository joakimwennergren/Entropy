#pragma once

#include <servicelocators/servicelocator.hpp>

class IPhysics2D : public IService
{
public:
    virtual ~IPhysics2D() = default;
    virtual b2World *Get() = 0;
    virtual b2Body *CreateGround(float x, float y, float w, float h) = 0;
    virtual b2Body *CreateDynamicBody(float x, float y, float w, float h) = 0;
    virtual b2Body *CreateSensorBody(float x, float y, float w, float h,
                                     b2Vec2 *pos) = 0;
};