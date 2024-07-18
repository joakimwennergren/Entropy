#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include <config.hpp>
#include <glm/glm.hpp>
#include <graphics/vulkan/devices/logical_device.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>
#include <vulkan/vulkan.hpp>

namespace Entropy {
namespace Cameras {

struct Camera {
  virtual void Test() = 0;
};

} // namespace Cameras
} // namespace Entropy
