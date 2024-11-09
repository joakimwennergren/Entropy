#pragma once

#include "icamera_manger.hpp"

namespace Entropy::Cameras {
  struct CameraManager final : ServiceBase<ICameraManager> {
    CameraManager() = default;
  };
} // namespace Entropy::Cameras
