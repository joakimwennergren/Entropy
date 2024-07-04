#include "application.hpp"
#include "graphics/renderers/vulkan_renderer.hpp"
#include "services/allocator_service.hpp"
#include "services/logical_device_service.hpp"
#include "services/physical_device_service.hpp"
#include "services/vk_instance_service.hpp"
#include "services/vulkan_backend_service.hpp"
#include "services/vulkan_render_service.hpp"
#include <graphics/vulkan/vulkan_backend.hpp>

using namespace Entropy;

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX) ||                    \
    defined(BUILD_FOR_WINDOWS)

Application::Application() {

  /*

// Seed random
srand(static_cast<unsigned>(time(0)));

// Initialize GLFW
if (!glfwInit()) {
  spdlog::critical("Could not initialize GLFW.");
  exit(EXIT_FAILURE);
}

// Create the window
glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
_window = glfwCreateWindow(1000, 640, "Entropy application", NULL, NULL);

if (!_window) {
  spdlog::critical("Could not create window.");
  glfwTerminate();
  exit(EXIT_FAILURE);
}

// Bind window callbacks
glfwSetWindowUserPointer(_window, this);
glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
glfwSetMouseButtonCallback(_window, mouse_button_callback);
glfwSetKeyCallback(_window, keyCallback);
glfwSetCursorPosCallback(_window, cursor_position_callback);
glfwSetCharCallback(_window, character_callback);
glfwSetScrollCallback(_window, scroll_callback);
glfwSetWindowIconifyCallback(_window, window_iconify_callback);
glfwSetWindowContentScaleCallback(_window, window_content_scale_callback);

// Get initial window framebuffer size
int width, height;
glfwGetFramebufferSize(_window, &width, &height);
VkExtent2D frame = {(uint32_t)width, (uint32_t)height};

*/

  /*
    VkExtent2D frame = {(uint32_t)400, (uint32_t)400};
    // Create 1ms Timer
    _timer = std::make_unique<Timer>(1.0);

    // Create items for vulkan
    //_vkInstance = std::make_shared<VulkanInstance>();
    //_windowSurface = std::make_shared<WindowSurface>(_vkInstance, _window);
    _physicalDevice = std::make_shared<PhysicalDevice>(instance);
    _logicalDevice = std::make_shared<LogicalDevice>(_physicalDevice);
    _allocator =
        std::make_shared<Allocator>(instance, _logicalDevice, _physicalDevice);
    _swapChain = std::make_shared<Swapchain>(
        _physicalDevice->Get(), _logicalDevice->Get(), frame, 800, 800);
    auto commandPool =
        std::make_shared<CommandPool>(_logicalDevice, _physicalDevice);
    _descriptorPool = std::make_shared<DescriptorPool>(_logicalDevice);

    // Add services to service locator
    serviceLocator = std::make_shared<ServiceLocator>();

    _keyboard = std::make_shared<Keyboard>(serviceLocator);
    _camera = std::make_shared<Cam>(glm::vec3(0.0f, 0.0f, 3.0f));
    serviceLocator->AddService(_camera);
    serviceLocator->AddService(_keyboard);
    serviceLocator->AddService(_physicalDevice);
    serviceLocator->AddService(_logicalDevice);
    serviceLocator->AddService(_allocator);
    serviceLocator->AddService(_descriptorPool);
    serviceLocator->AddService(_swapChain);
    serviceLocator->AddService(commandPool);
    // serviceLocator->AddService(_windowSurface);

    physics2d = std::make_shared<Physics2D>(serviceLocator);
    serviceLocator->AddService(physics2d);

    mouse = std::make_shared<Mouse>(_window);
    serviceLocator->AddService(mouse);

    lua = std::make_shared<Lua>(serviceLocator);
    serviceLocator->AddService(lua);

    world = std::make_shared<World>();
    serviceLocator->AddService(world);

    auto queueSync = std::make_shared<QueueSync>();
    serviceLocator->AddService(queueSync);

    physics3d = std::make_shared<Physics3D>(&world->gameWorld, serviceLocator);
    serviceLocator->AddService(physics3d);

    _renderer = std::make_shared<Renderer>(serviceLocator, &world->gameWorld,
                                           xscale, yscale);

    //   auto &io = ImGui::GetIO();
    //   io.DisplaySize = ImVec2(width, height);

    // auto monitor = glfwGetPrimaryMonitor();
    // float xs, ys;
    // glfwGetWindowContentScale(_window, &xs, &ys);

    // xscale = xs;
    // yscale = ys;

    _renderer->_camera->setPerspective(60.0f, (float)400 / (float)400, 1.0f,
                                       100000.0f);
    */
}

// void Application::ExecuteScripts(std::shared_ptr<SceneGraph> sceneGraph,
// std::shared_ptr<Lua> lua)
// {
//     for (auto renderable : sceneGraph->renderables)
//     {
//         if (!renderable->script->hasExecuted)
//         {
//             if (renderable->script->scriptFile.length() > 0)
//             {
//                 lua->ExecuteScript("", renderable->script->scriptFile,
//                 renderable->script->environment);
//             }
//             else
//             {
//                 lua->ExecuteScript(renderable->script->script, "",
//                 renderable->script->environment);
//             }

//             renderable->script->hasExecuted = true;
//         }

//         auto onRenderFunc = renderable->script->environment["OnRender"];

//         // if (onRenderFunc != nullptr)
//         //{
//         //     onRenderFunc();
//         // }
//     }
// }

Application::~Application() {
  glfwDestroyWindow(_window);
  glfwTerminate();
}

void Application::Run() {

  // auto lua = serviceLocator->GetService<Lua>();
  // auto physics3d = serviceLocator->GetService<Physics3D>();
  // auto physicalDevice = serviceLocator->GetService<PhysicalDevice>();
  // auto queueSync = serviceLocator->GetService<QueueSync>();

  this->OnInit();

  _timer->Start();
  _lastTick = (float)_timer->GetTick();

  // _camera->Position = glm::vec3(-43.0f, -24.0f, -40.0f);
  // _camera->Pitch = 22.69f;
  // _camera->Yaw = 44.20f;

  while (!glfwWindowShouldClose(_window)) {

    // spdlog::info("ms/frame = {}", _deltaTime);

    // Calculate delta time
    _deltaTime = _timer->GetTick() - _lastTick;
    _lastTick = _timer->GetTick();

    // Update screen dimensions
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);
    int contentScaleWidth, contentScaleHeight;

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX) ||                    \
    defined(BUILD_FOR_WINDOWS)
    while (isMinimized) {
      glfwWaitEvents();
    }
#endif

    this->OnRender(_deltaTime);
    // this->_renderer->Render(width * xscale, height * yscale, xscale, yscale);
    //  On render

    // Increment current frame

    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(_window, true);

    /*
    if (io.MouseDown[2] || io.MouseDown[1]) {
      float speed = 0.1;
      if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
        speed = 0.8;
      }
      if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        _camera->ProcessKeyboard(FORWARD, speed);
      if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        _camera->ProcessKeyboard(BACKWARD, speed);
      if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        _camera->ProcessKeyboard(LEFT, speed);
      if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        _camera->ProcessKeyboard(RIGHT, speed);
    }
    */

    //     float timeStep = 1.0f / 60.0f;
    // int32 velocityIterations = 6;
    // int32 positionIterations = 2;

    /*
    // physics2d->world->Step(timeStep, velocityIterations, positionIterations);
    physics3d->GetWorld()->stepSimulation(
        1.0f /
        60.0f); // Example: step the simulation with a time step of 1/60 seconds

    // ExecuteScripts(sceneGraph, lua);

    _camera->updateCameraVectors();
    _renderer->_camera->setPerspective(
        _camera->Zoom, (float)width / (float)height, 1.0f, 100000.0f);

    // Poll events
    glfwPollEvents();

    for (auto deletable :
         serviceLocator->GetService<QueueSync>()->_deletables) {
      deletable.destruct();
      serviceLocator->GetService<QueueSync>()->_deletables.pop_back();
    }
      */
  }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  /*
ImGuiIO &io = ImGui::GetIO();
if (action == GLFW_PRESS)
io.KeysDown[key] = true;
if (action == GLFW_RELEASE)
io.KeysDown[key] = false;
// Modifiers are not reliable across systems
io.KeyCtrl =
io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
io.KeyShift =
io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
io.KeySuper =
io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

*/
}

void character_callback(GLFWwindow *window, unsigned int codepoint) {}

void cursor_position_callback(GLFWwindow *window, double xposIn,
                              double yposIn) {
  auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

  if (app == nullptr) {
    return;
  }

  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (app->firstMouse) {
    app->lastX = xpos;
    app->lastY = ypos;
    app->firstMouse = false;
  }

  float xoffset = xpos - app->lastX;
  float yoffset =
      app->lastY - ypos; // reversed since y-coordinates go from bottom to top

  app->lastX = xpos;
  app->lastY = ypos;

  // app->_camera->ProcessMouseMovement(xoffset, yoffset);
}

void framebufferResizeCallback(GLFWwindow *window, int width, int height) {
  auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

  GLFWmonitor *primary = glfwGetPrimaryMonitor();

  // app->GetRenderer()->Render(width * app->xscale, height * app->yscale,
  //                            app->xscale, app->yscale);
  //  app->_renderer->_camera->setPerspective(
  //      app->_camera->Zoom, (float)width / (float)height, 1.0f, 100000.0f);
  app->OnRender(0.0);
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  /*
  auto &io = ImGui::GetIO();
  auto app = reinterpret_cast<Application
  *>(glfwGetWindowUserPointer(window));

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    io.MouseDown[0] = true;
  if (button == GLFW_MOUSE_BUTTON_LEFT && action != GLFW_PRESS)
    io.MouseDown[0] = false;

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    io.MouseDown[2] = true;
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action != GLFW_PRESS)
    io.MouseDown[2] = false;
    */
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
  // app->_camera->ProcessMouseScroll((float)yoffset);
}

void window_iconify_callback(GLFWwindow *window, int iconified) {
  auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

  if (iconified) {
    app->isMinimized = true;
  } else {
    app->isMinimized = false;
  }
}

void window_content_scale_callback(GLFWwindow *window, float xscale,
                                   float yscale) {
  auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

  app->xscale = xscale;
  app->yscale = yscale;
}

#endif
