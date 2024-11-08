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
  class OrthographicCamera final : public Camera {
    /**
     * @brief Default constructor for the OrthographicCamera class.
     *
     * Initializes the view matrix using the glm::lookAt function, setting the
     * camera position to (0.0f, 0.0f, 256.0f) and looking at the origin (0.0f, 0.0f, 0.0f)
     * with the up direction being the positive y-axis (0.0f, 1.0f, 0.0f).
     */
  public:
    OrthographicCamera() {
      matrices.view =
          glm::lookAt(glm::vec3(0.0f, 0.0f, 256.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f));
    }

    float fov{};
    float znear{}, zfar{};

    void Test() {
    }

    enum CameraType { lookat, firstperson };

    CameraType type = CameraType::firstperson;

    glm::vec3 rotation = glm::vec3();
    glm::vec3 position = glm::vec3();
    glm::vec4 viewPos = glm::vec4();

    float rotationSpeed = 1.0f;
    float movementSpeed = 1.0f;

    bool updated = true;
    bool flipY = false;

    struct {
      glm::mat4 perspective;
      glm::mat4 view;
    } matrices{};

    struct {
      bool left = false;
      bool right = false;
      bool up = false;
      bool down = false;
    } keys;

    const float PPM = 100.0f; // Pixels Per Meter

    void setPerspective(float fov, int width, int height, float znear,
                        float zfar) {
      // Vulkan-trick because GLM was written for OpenGL, and Vulkan uses
      // a right-handed coordinate system instead. Without this correction,
      // geometry will be y-inverted in screen space, and the coordinate space
      // will be left-handed. Described at:
      // https://matthewwellings.com/blog/the-new-vulkan-coordinate-system/
      glm::mat4 correction(
        glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, -1.0f, 0.0f, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 0.5f, 1.0f));
      matrices.perspective = glm::ortho(0.0f, (float) width / PPM,
                                        (float) height / PPM, 0.0f, znear, zfar);
      // matrices.perspective =
      //     correction * glm::ortho(-1.0f, 1.0f, -aspect, aspect, znear, zfar);
    };
  };
};


