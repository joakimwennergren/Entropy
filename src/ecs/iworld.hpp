#pragma once

#include <servicelocators/servicelocator.hpp>

class IWorld : public IService
{
public:
    virtual ~IWorld() = default;
    virtual std::shared_ptr<flecs::world> Get() = 0;
};