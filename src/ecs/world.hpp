#pragma once

#include "iworld.hpp"
#include <flecs.h>

namespace Entropy::ECS {
 struct World final : public ServiceBase<IWorld> {
  /**
   * Constructor for the World class.
   * Initializes the gameWorld object by creating a new instance of flecs::world.
   *
   * @return A pointer to the newly created flecs::world instance.
   */
  World() { gameWorld = new flecs::world(); }

  /**
   * Destructor for the World class.
   * Cleans up the gameWorld object by deleting the instance of flecs::world.
   */
  ~World() override { delete gameWorld; }

  flecs::world *Get() override { return gameWorld; }

 private:
  flecs::world *gameWorld;
 };
} // namespace Entropy::ECS
