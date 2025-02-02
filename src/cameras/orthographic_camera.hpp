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
      cameraPosition = glm::vec3(0.0f, 0.0f, 256.0f);
      matrices.view =
          lookAt(glm::vec3(0.0f, 0.0f, 256.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                 glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void Test() override {
    }

    // Camera state
    glm::vec3 cameraPosition{}; // Camera's world position

    float zoomFactor = 1.0f;

    // Function to update zoom factor (call this based on user input)
    void Zoom(const float delta) {
      zoomFactor += delta;
      if (zoomFactor < 0.1f) zoomFactor = 0.1f; // Prevent zooming too far in
    }


    struct {
      glm::mat4 perspective;
      glm::mat4 view;
    } matrices{};

    // Pan the camera by offsetting the position
    void PanCamera(const float deltaX, const float deltaY) {
      cameraPosition.x = deltaX;
      cameraPosition.y = deltaY;

      // Recalculate view matrix
      // Recalculate the view matrix
      /*
      matrices.view = lookAt(
        cameraPosition, // Camera position
        cameraPosition + glm::vec3(0.0f, 0.0f, -1.0f), // Target (forward direction)
        glm::vec3(0.0f, 1.0f, 0.0f) // Up vector
      );
      */
    }

    void setPerspective(const int width, const int height, const float xscale, const float yscale, const float znear,
                        const float zfar) {
      // Calculate the orthographic projection matrix
      const auto perspective = glm::ortho(
        cameraPosition.x, // Left
        static_cast<float>(width) / zoomFactor + cameraPosition.x, // Right
        static_cast<float>(height) / zoomFactor + cameraPosition.y, // Top
        cameraPosition.y, // Bottom
        znear, // Near plane
        zfar // Far plane
      );

      // Calculate the center of the view based on the left, right, top, and bottom values
      float centerX = (cameraPosition.x + (static_cast<float>(width) / zoomFactor + cameraPosition.x)) / 2.0f;
      float centerY = (cameraPosition.y + (static_cast<float>(height) / zoomFactor + cameraPosition.y)) / 2.0f;

      // Apply scaling centered around the view's center
      glm::mat4 scaledPerspective = perspective;
      scaledPerspective = glm::translate(scaledPerspective, glm::vec3(centerX, centerY, 0.0f)); // Translate to center
      scaledPerspective = glm::scale(scaledPerspective, glm::vec3(xscale, yscale, 1.0f)); // Scale
      scaledPerspective = glm::translate(scaledPerspective, glm::vec3(-centerX, -centerY, 0.0f)); // Translate back

      // Store the result in the matrix
      matrices.perspective = scaledPerspective;
    }
  };
};
