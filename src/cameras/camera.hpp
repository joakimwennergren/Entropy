#ifndef CAMERA_HPP
#define CAMERA_HPP

namespace Entropy::Cameras {
  struct Camera {
    virtual ~Camera() = default;

    virtual void Test() = 0;
  };
} // namespace Entropy::Cameras

#endif // CAMERA_HPP
