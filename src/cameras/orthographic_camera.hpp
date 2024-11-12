#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <cameras/camera.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Entropy::Cameras {
  /**
   * @class OrthographicCamera
   * @brief Represents a camera that uses orthographic projection.
   *
   * This class inherits from Camera and provides functionality for orthographic
   * projection, including updating the view and perspective matrices, and handling
   * camera movement and rotation.
   *
   * The orthographic camera projects objects onto the screen without perspective,
   * meaning objects retain the same size regardless of their depth from the camera.
   */
  struct OrthographicCamera final : Camera {
    /**
     * @brief Default constructor for the OrthographicCamera class.
     *
     * Initializes the view matrix using the glm::lookAt function, setting the
     * camera position to (0.0f, 0.0f, 256.0f) and looking at the origin (0.0f, 0.0f, 0.0f)
     * with the up direction being the positive y-axis (0.0f, 1.0f, 0.0f).
     */
    OrthographicCamera() {
      matrices.view =
          lookAt(glm::vec3(0.0f, 0.0f, 256.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                 glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void Test() {
    }

    struct {
      glm::mat4 perspective;
      glm::mat4 view;
    } matrices{};

    const float PPM = 100.0f;

    void setPerspective(const int width, const int height, const float znear,
                        const float zfar) {
      matrices.perspective = glm::ortho(0.0f, static_cast<float>(width) / PPM,
                                        static_cast<float>(height) / PPM, 0.0f, znear, zfar);
    };
  };
};
