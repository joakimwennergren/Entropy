#pragma once

namespace Entropy::Cameras {
  struct Camera {
    virtual ~Camera() = default;

    const float PPM = 100.0;

    virtual void Test() = 0;
  };
} // namespace Entropy::Cameras
