#pragma once

#include <flecs.h>
#include <servicelocators/servicelocator.hpp>

/**
 * Interface for a world object that extends the IService interface.
 */
namespace Entropy {
namespace ECS {
struct IWorld : public IService {
  /**
   * Virtual destructor for the IWorld interface.
   *
   * @returns None
   */
  virtual ~IWorld() = default;
  /**
   * Abstract method to get a pointer to a flecs world.
   *
   * @returns A pointer to a flecs world.
   */
  virtual flecs::world *Get() = 0;
};
} // namespace ECS
} // namespace Entropy