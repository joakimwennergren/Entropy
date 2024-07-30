#pragma once

#include <servicelocators/servicelocator.hpp>

class IWorld : public IService
{
public:
    virtual ~IWorld() = default;
    virtual flecs::world *Get() = 0;
};