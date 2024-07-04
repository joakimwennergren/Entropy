
#pragma once

#include <ecs/world.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::ECS;

namespace Entropy {
namespace Graphics {
namespace Services {

struct WorldService : kgr::single_service<World> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
