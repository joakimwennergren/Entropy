#pragma once

#include <cameras/camera.hpp>
#include <servicelocators/servicelocator.hpp>

class ICameraManager : public IService {
public:
    ~ICameraManager() override = default;


    Entropy::Cameras::Camera *currentCamera;
};
