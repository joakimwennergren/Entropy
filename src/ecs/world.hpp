#pragma once
#include <flecs/flecs.h>

namespace Entropy {
namespace ECS {
struct World {

  World() { gameWorld = new flecs::world(); }

  flecs::world *gameWorld;
};
} // namespace ECS
} // namespace Entropy