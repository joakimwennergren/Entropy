#pragma once
#include <vector>
#include <cameras/camera.hpp>

namespace Entropy
{
  namespace Cameras
  {

    struct CameraManager
    {

      CameraManager() {}

      std::vector<std::shared_ptr<Camera>> cameras;
      Camera *currentCamera;
    };

  } // namespace Cameras
} // namespace Entropy
