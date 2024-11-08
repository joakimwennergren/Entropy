#pragma once

#include <config.hpp>
#include <entropy.hpp>
#include <graphics/renderers/vulkan_renderer.hpp>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) ||                  \
    defined(BUILD_FOR_LINUX)

#include <GLFW/glfw3.h>

/**
 * Callback function for handling framebuffer resize events.
 *
 * This function is called whenever the framebuffer of the specified window is resized.
 * It updates the application's resolution and sets a flag indicating that resizing is required.
 *
 * @param window The GLFW window that received the resize event.
 * @param width The new width of the framebuffer.
 * @param height The new height of the framebuffer.
 */
void framebuffer_resize_callback(GLFWwindow *window, int width, int height);

/**
 * Callback function for handling GLFW key events.
 *
 * @param window The GLFW window that received the event.
 * @param key The keyboard key that was pressed or released.
 * @param scancode The system-specific scancode of the key.
 * @param action The key action: GLFW_PRESS, GLFW_RELEASE, or GLFW_REPEAT.
 * @param mods Bit field describing which modifier keys were held down.
 */
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods);

/**
 * Callback function to handle cursor position events in a GLFW window.
 *
 * This function is called whenever the cursor is moved within the window's
 * client area. It updates the application's internal state with the new
 * cursor position and calculates the offset from the last known cursor
 * position. This offset can be used for implementing functionalities like
 * camera movement or object dragging.
 *
 * @param window The GLFW window in which the cursor position event occurred.
 * @param xposIn The new x-coordinate of the cursor.
 * @param yposIn The new y-coordinate of the cursor.
 */
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);

/**
 * @brief Callback function that handles window refresh events.
 *
 * This function is invoked whenever the window needs to be refreshed.
 * It ensures the contents of the window are properly redrawn.
 *
 * @param window A pointer to the GLFW window that requires refreshing.
 */
void window_refresh_callback(GLFWwindow *window);

/**
 * @brief Callback for handling mouse button events.
 *
 * This function is triggered whenever a mouse button is pressed or released within the application window.
 * It updates the application's internal mouse state based on which button was interacted with and the type of action performed.
 *
 * @param window Pointer to the GLFW window that received the event.
 * @param button Identifier of the mouse button that was pressed or released.
 * @param action Indicates whether the button was pressed or released.
 * @param mods Bitfield describing which modifier keys were held down.
 */
void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods);

/**
 * Callback function for handling Unicode character input events.
 *
 * @param window A pointer to the GLFW window that received the event.
 * @param codepoint The Unicode code point of the character.
 */
void character_callback(GLFWwindow *window, unsigned int codepoint);

/**
 * Callback function for handling scroll events in a GLFW window.
 *
 * @param window A pointer to the GLFWwindow in which the scroll event occurred.
 * @param xoffset The scroll offset along the x-axis.
 * @param yoffset The scroll offset along the y-axis.
 */
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

/**
 * Callback function called when a GLFW window is iconified or restored.
 *
 * @param window A pointer to the GLFW window that received the event.
 * @param iconified A flag indicating whether the window was iconified (1) or restored (0).
 */
void window_iconify_callback(GLFWwindow *window, int iconified);

/**
 * Callback function for handling changes in the content scale of a specified window.
 * This function is called when the content scale of the window changes, typically
 * due to a change in the display scaling settings for high-DPI displays.
 *
 * @param window Pointer to the GLFW window that received the event.
 * @param xscale The new x-axis content scale of the window.
 * @param yscale The new y-axis content scale of the window.
 */
void window_content_scale_callback(GLFWwindow *window, float xscale,
                                   float yscale);

namespace Entropy::EntryPoints {
 class Application {
 public:
  /**
  * Constructor for the Application class.
  *
  * This constructor initializes the GLFW library, creates a GLFW window, and sets various
  * callbacks for handling window events. It also initializes random seeding, sets up a timer,
  * and registers several services, including Vulkan services, ECS, 2D physics, scripting,
  * camera management, and the Vulkan renderer. Additionally, it creates a window surface and
  * builds the swapchain and framebuffers required for the Vulkan renderer.
  *
  * Upon failure to initialize GLFW or create the window, the constructor logs a critical
  * error message and exits the application.
  */
  Application();

  /**
   * Destructor for the Application class.
   *
   * This destructor is responsible for cleaning up resources associated with the
   * application. It destroys the GLFW window and terminates the GLFW library.
   */
  virtual ~Application();

  /**
   * Pure virtual function to initialize the application.
   *
   * This function is meant to be overridden by derived classes to perform any
   * necessary initialization steps specific to the application.
   * It is called once before the main loop of the application starts.
   */
  virtual void OnInit() = 0;

  /**
   * Pure virtual function that must be implemented to handle rendering operations.
   *
   * This function is called every frame and is intended to encompass all necessary
   * drawing and rendering logic, using the provided delta-time to ensure frame-rate
   * independent updates.
   *
   * @param deltaTime The time difference between the current frame and the previous frame in seconds.
   */
  virtual void OnRender(float deltaTime) = 0;

  std::vector<flecs::entity> CreateGrid(std::shared_ptr<IWorld> world,
                                        float screenWidth, float screenHeight);

  void Run();

  bool firstMouse{};
  float lastX = 0.0;
  float lastY = 0.0;
  // @todo look over if this should be protected..
  bool isResizing = false;

  float mouse_x{};
  int nbFrames = 0;
  float lastTime{};
  float mouse_y{};
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
} // namespace Entropy::EntryPoints


#endif
