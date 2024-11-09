#pragma once

namespace Entropy::Cameras {
  struct Camera {
    virtual ~Camera() = default;

    virtual void Test() = 0;
  };
} // namespace Entropy::Cameras
