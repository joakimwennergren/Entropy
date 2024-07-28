#pragma once
#include <vector>
#include <cameras/camera.hpp>
#include "icamera_manger.hpp"

namespace Entropy
{
  namespace Cameras
  {

    struct CameraManager : public ServiceBase<ICameraManager>
    {
      CameraManager() {}
    };

  } // namespace Cameras
} // namespace Entropy
