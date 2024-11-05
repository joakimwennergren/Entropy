#pragma once
#include "icamera_manger.hpp"
#include <cameras/camera.hpp>

namespace Entropy {
namespace Cameras {
struct CameraManager : public ServiceBase<ICameraManager> {
  CameraManager() {}
};

} // namespace Cameras
} // namespace Entropy
