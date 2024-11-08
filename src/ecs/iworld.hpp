
#pragma once

#include <flecs.h>
#include <servicelocators/servicelocator.hpp>

namespace Entropy::ECS {
 struct IWorld : IService {
  /**
   * @brief Destructor for the IWorld interface.
   *
   * This is a defaulted override of the base class destructor. It ensures
   * proper cleanup of resources when an instance of an IWorld-derived class is destroyed.
   */
  ~IWorld() override = default;

  virtual flecs::world *Get() = 0;
 };
} // namespace Entropy::ECS
