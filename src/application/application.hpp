#pragma once

#include "graphics/renderers/vulkan_renderer.hpp"
#include <entropy.hpp>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) ||                  \
    defined(BUILD_FOR_LINUX)

#include <GLFW/glfw3.h>

void framebufferResizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void window_refresh_callback(GLFWwindow *window);
void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods);
void character_callback(GLFWwindow *window, unsigned int codepoint);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void window_iconify_callback(GLFWwindow *window, int iconified);
void window_content_scale_callback(GLFWwindow *window, float xscale,
                                   float yscale);

namespace Entropy {
namespace EntryPoints {
class Application {
public:
  Application();
  ~Application();

  virtual void OnInit() = 0;
  virtual void OnRender(float deltaTime) = 0;
  std::vector<flecs::entity> CreateGrid(std::shared_ptr<IWorld> world,
                                        float screenWidth, float screenHeight);

  void Run();

  bool firstMouse;
  float lastX = 0.0;
  float lastY = 0.0;
  // @todo look over if this should be protected..
  bool isResizing = false;

  float mouse_x;
  int nbFrames = 0;
  float lastTime;
  float mouse_y;
  bool mouse0_state = false;
  bool isMinimized = false;
  float xscale = 1.0;
  float yscale = 1.0;

  int screen_width = 0;
  int screen_height = 0;
  int old_screen_width = 0;
  int old_screen_height = 0;
  bool needResize = false;

protected:
  GLFWwindow *_window;
  std::shared_ptr<Renderers::VulkanRenderer> _renderer;

private:
  VkSurfaceKHR _surface = VK_NULL_HANDLE;
  std::unique_ptr<Entropy::Timing::Timer> _timer;
  float _lastTick = 0.0f;
  float _deltaTime = 0.0f;
};
} // namespace EntryPoints
} // namespace Entropy

#endif
