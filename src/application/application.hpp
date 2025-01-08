#ifndef ENTROPY_APPLICATION_H
#define ENTROPY_APPLICATION_H

#include <config.hpp>
#include <entropy.hpp>
#include <graphics/renderers/vulkan_renderer.hpp>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || \
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
 * @param xpos The new x-coordinate of the cursor.
 * @param ypos The new y-coordinate of the cursor.
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
void MouseButtonCallback(GLFWwindow *window, int button, int action,
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
void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

/**
 * Callback function called when a GLFW window is iconified or restored.
 *
 * @param window A pointer to the GLFW window that received the event.
 * @param iconified A flag indicating whether the window was iconified (1) or restored (0).
 */
void WindowIconifyCallback(GLFWwindow *window, int iconified);

/**
 * Callback function for handling changes in the content scale of a specified window.
 * This function is called when the content scale of the window changes, typically
 * due to a change in the display scaling settings for high-DPI displays.
 *
 * @param window Pointer to the GLFW window that received the event.
 * @param xscale The new x-axis content scale of the window.
 * @param yscale The new y-axis content scale of the window.
 */
void WindowContentScaleCallback(GLFWwindow *window, float xscale,
                                float yscale);

namespace Entropy::EntryPoints
{
  class Application
  {
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
     * @param screen_width
     * @param screen_height
     * @param deltaTime The time difference between the current frame and the previous frame in seconds.
     */
    virtual void OnRender(float screen_width, float screen_height, float deltaTime) = 0;

    /**
     * Runs the application's main loop.
     *
     * This method initializes the application, begins the main loop, and handles
     * events such as rendering, script execution, and input processing.
     * - Calls the OnInit function to perform initial setup.
     * - Continuously updates the timer and calculates the delta time for frame updates.
     * - Manages the rendering process and script callbacks for rendering and input.
     * - Handles window events and maintains the application state until the window should close.
     */
    void Run();

    std::shared_ptr<Renderers::VulkanRenderer> GetVulkanRenderer() { return _renderer; }

    int keyDown = -1;

    float scrollX = 0.0f;

    float scrollY = 0.0f;

    /**
     * Scaling factor along the x-axis.
     *
     * This member variable holds the value for the scaling factor applied
     * to the x-axis content within the application window.
     * It is updated through the window content scale callback to reflect changes
     * in the display scaling settings.
     */
    float xscale = 1.0;
    /**
     * Scaling factor for the application's content along the Y-axis.
     *
     * This value determines how much the content is scaled in the vertical direction.
     * It is typically updated by the window content scale callback to reflect changes
     * in the window's scaling settings.
     */
    float yscale = 1.0;

    /**
     * Stores the width of the application's screen.
     *
     * This variable holds the current width of the screen in pixels. It is primarily used
     * for configuring rendering settings and handling window resize events.
     */
    int screen_width = 0;
    /**
     * Global variable that stores the height of the screen.
     *
     * This variable is used within the Application class to keep track of the current
     * screen height, which can be referenced in rendering and layout calculations.
     */
    int screen_height = 0;
    /**
     * Flag indicating whether the application needs to be resized.
     *
     * This boolean flag is set to true whenever a resize event occurs.
     * It is checked in the main loop to determine if the framebuffer
     * and related resources need to be updated to match the new window size.
     */
    bool needResize = false;
    /**
     * A flag indicating whether the application window is minimized.
     *
     * This boolean variable is set to true when the application window is minimized
     * and set to false when it is restored. It helps to manage the application's
     * state and event handling related to window size changes.
     */
    bool isMinimized = false;

    std::unordered_map<uint32_t, flecs::entity> entityMap;

    uint32_t physics2dindex = 0;

  private:
    /**
     * Shared pointer to the VulkanRenderer instance.
     *
     * This variable holds an instance of VulkanRenderer, responsible for managing
     * the rendering process using the Vulkan API. By using a unique pointer,
     * ownership and lifecycle management are ensured, preventing memory leaks
     * and ensuring the renderer is properly destroyed when no longer needed.
     */
    std::shared_ptr<Renderers::VulkanRenderer> _renderer;

    /**
     * Unique pointer to a Timer object.
     *
     * This variable holds a unique pointer to an instance of the Timer class.
     * It is used to manage the timer's lifecycle and ensure that only one instance
     * of the timer exists throughout its scope. The Timer object is responsible for
     * handling timed events within the application.
     */
    std::unique_ptr<Timer> _timer;
    /**
     * Pointer to the main GLFW window object.
     *
     * This variable holds a reference to the main window used by the application.
     * It is initialized to nullptr and is assigned a valid window object
     * once the window is created. This window serves as the primary context
     * for rendering and event handling in the application.
     */
    GLFWwindow *_window = nullptr;
    /**
     * Vulkan surface handle used for presentation.
     *
     * This variable holds the handle to a Vulkan surface, which represents an abstraction for a
     * platform-specific surface where rendering can occur. It is typically used for presenting rendered images to a display.
     * The initial value is set to VK_NULL_HANDLE, indicating that it is not yet initialized.
     */
    VkSurfaceKHR _surface = VK_NULL_HANDLE;
    /**
     * Stores the timestamp of the last game tick.
     *
     * This variable keeps track of the time at which the last game tick was processed.
     * It is used to calculate the time elapsed between ticks, facilitating updates
     * and synchronization within the game loop.
     */
    float _lastTick = 0.0f;
    /**
     * Variable that represents the time difference between the current frame and the previous frame.
     *
     * This value is typically used to achieve frame rate independent movement and calculations
     * in real-time applications, such as games or simulations.
     */
    float _deltaTime = 0.0f;
  };
} // namespace Entropy::EntryPoints
#endif

#endif // ENTROPY_APPLICATION_H