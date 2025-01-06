#pragma once

#include <sol/sol.hpp>
#include <string>

namespace Entropy {
namespace Scripting {
class Script {
public:
  Script() = default;
  std::string script;
  std::string scriptFile;
  sol::environment environment;
  bool hasExecuted = false;

private:
};
} // namespace Scripting
} // namespace Entropy
