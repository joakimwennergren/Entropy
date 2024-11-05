#pragma once

#include "iworld.hpp"
#include <flecs.h>

namespace Entropy {
namespace ECS {
/**
 * Definition of a struct World that inherits from ServiceBase<IWorld>.
 */
struct World : public ServiceBase<IWorld> {
  /**
   * Constructor for the World class.
   * Initializes a new flecs world for the game.
   *
   * @returns None
   */
  World() { gameWorld = new flecs::world(); }
  /**
   * Destructor for the World class.
   * Deletes the gameWorld object to free up memory.
   */
  ~World() { delete gameWorld; }
  /**
   * Get the flecs world instance.
   *
   * @return A pointer to the flecs world instance.
   */
  inline flecs::world *Get() override { return gameWorld; }

private:
  /**
   * Pointer to the flecs world representing the game world.
   */
  flecs::world *gameWorld;
};
} // namespace ECS
} // namespace Entropy