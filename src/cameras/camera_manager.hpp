#pragma once

#include <cameras/camera.hpp>

namespace Entropy {
namespace Cameras {

struct CameraManager {

  CameraManager() {}

  std::vector<std::shared_ptr<Camera>> cameras;
  std::shared_ptr<Camera> currentCamera;
};

} // namespace Cameras
} // namespace Entropy
