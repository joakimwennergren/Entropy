#pragma once

#include <servicelocators/servicelocator.hpp>

class ILua : public IService
{
public:
    virtual ~ILua() = default;
    virtual sol::state *Get() = 0;
};