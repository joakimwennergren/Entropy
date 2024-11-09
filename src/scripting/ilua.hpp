#pragma once

#include <servicelocators/servicelocator.hpp>
#include <sol/sol.hpp>

class ILua : public IService {
public:
  virtual ~ILua() = default;

  virtual sol::state *Get() = 0;
};
