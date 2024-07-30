#pragma once

#include <flecs/flecs.h>
#include "iworld.hpp"

namespace Entropy
{
  namespace ECS
  {
    struct World : public ServiceBase<IWorld>
    {
      World() { gameWorld = new flecs::world(); }
      inline flecs::world *Get() override { return gameWorld; }
      flecs::world *gameWorld;
    };
  } // namespace ECS
} // namespace Entropy