
#pragma once

#include "services/camera_manager_service.hpp"
#include <kangaru/kangaru.hpp>
#include <scripting/lua.hpp>
#include <services/world_service.hpp>

using namespace Entropy::Scripting;

namespace Entropy {
namespace Graphics {
namespace Services {

struct ScriptingService
    : kgr::single_service<Lua, kgr::dependency<WorldService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
