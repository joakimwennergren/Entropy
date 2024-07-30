#pragma once
#include <cameras/camera.hpp>
#include <servicelocators/servicelocator.hpp>

class ICameraManager : public IService
{
public:
    virtual ~ICameraManager() = default;
    Entropy::Cameras::Camera *currentCamera;
};