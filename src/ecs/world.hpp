#pragma once

#include <flecs/flecs.h>
#include "iworld.hpp"

namespace Entropy
{
  namespace ECS
  {
    struct World : public ServiceBase<IWorld>
    {

      World() { gameWorld = std::make_shared<flecs::world>(); }
      inline std::shared_ptr<flecs::world> Get() override { return gameWorld; }
      std::shared_ptr<flecs::world> gameWorld;
    };
  } // namespace ECS
} // namespace Entropy