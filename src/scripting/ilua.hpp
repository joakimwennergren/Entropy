#pragma once

#include <servicelocators/servicelocator.hpp>
#include <sol/sol.hpp>

class ILua : public IService {
public:
  virtual ~ILua() = default;

  virtual void SetBasePath(const std::string &path) = 0;

  virtual sol::state *Get() = 0;
};
