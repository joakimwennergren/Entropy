#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

namespace Entropy
{
  namespace Data
  {

    struct UboDataDynamic
    {
      // glm::vec4 color;
      // glm::vec4 colorBorder;
      // glm::vec4 colorShadow;
      glm::mat4 perspective;
      glm::mat4 view;
      // glm::vec2 position;
      // glm::vec2 size;
      // glm::vec4 borderRadius;
    };

    struct InstanceData
    {
      glm::mat4 model;
      glm::vec4 color;
      glm::vec2 resolution;
      int type;
      int _pad[1];
    };

  } // namespace Data
} // namespace Entropy
