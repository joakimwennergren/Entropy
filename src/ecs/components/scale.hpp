#pragma once

#include <glm/glm.hpp>


namespace Entropy::Components {
  struct Dimension {
    glm::vec3 scale;
    glm::vec4 cornerRadiuses{0.0f};
  };
} // namespace Entropy::Components

