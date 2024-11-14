#include "application.hpp"
#include "ecs/iworld.hpp"
#include "glm/fwd.hpp"
#include "glm/gtx/string_cast.hpp"
#include "graphics/renderers/vulkan_renderer.hpp"
#include "vulkan/vulkan_core.h"
#include <graphics/primitives/primitives.hpp>


using namespace Entropy::EntryPoints;

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX) ||                    \
    defined(BUILD_FOR_WINDOWS)

Application::Application() {
  // Seed random
  srand(time(nullptr));

  // Initialize GLFW
  if (!glfwInit()) {
    spdlog::critical("Could not initialize GLFW");
    exit(EXIT_FAILURE);
  }

  // Create the window
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  _window = glfwCreateWindow(1000, 640, "Entropy application", nullptr, nullptr);

  if (!_window) {
    spdlog::critical("Could not create window.");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  // Bind window callbacks
  glfwSetWindowUserPointer(_window, this);
  glfwSetFramebufferSizeCallback(_window, framebuffer_resize_callback);
  glfwSetMouseButtonCallback(_window, mouse_button_callback);
  glfwSetKeyCallback(_window, keyCallback);
  glfwSetCursorPosCallback(_window, cursor_position_callback);
  glfwSetCharCallback(_window, character_callback);
  glfwSetScrollCallback(_window, scroll_callback);
  glfwSetWindowIconifyCallback(_window, window_iconify_callback);
  glfwSetWindowContentScaleCallback(_window, window_content_scale_callback);

  // Create 1ms Timer
  _timer = std::make_unique<Timer>(1.0);

  ServiceLocator *sl = ServiceLocator::GetInstance();

  // Vulkan services
  sl->registerService(std::make_shared<VulkanInstance>());
  sl->registerService(std::make_shared<PhysicalDevice>());
  sl->registerService(std::make_shared<LogicalDevice>());
  sl->registerService(std::make_shared<Allocator>());
  sl->registerService(std::make_shared<SwapChain>());
  sl->registerService(std::make_shared<CommandPool>());
  sl->registerService(std::make_shared<DescriptorPool>());
  sl->registerService(std::make_shared<PipelineCache>());

  // Cameras
  sl->registerService(std::make_shared<CameraManager>());
  const auto cameraManager = sl->getService<ICameraManager>();
  const auto camera = new OrthographicCamera();
  cameraManager->currentCamera = camera;

  // 2D Physics
  sl->registerService(std::make_shared<Physics2D>());

  // ECS
  sl->registerService(std::make_shared<World>());

  // Scripting
  sl->registerService(std::make_shared<Lua>());

  _renderer = std::make_unique<Renderers::VulkanRenderer>();

  if (const auto instance = sl->getService<IVulkanInstance>(); glfwCreateWindowSurface(instance->Get(), _window,
                                                                 nullptr,
                                                                 &_surface) !=
                                                               VK_SUCCESS) {
    std::cout << "Failed to create a window surface for platform 'MacOS'"
        << std::endl;
  }

  glfwGetFramebufferSize(_window, &screen_width, &screen_height);
  const VkExtent2D frame = {static_cast<uint32_t>(screen_width), static_cast<uint32_t>(screen_height)};

  _renderer->_swapchain->Build(_surface, frame, VK_NULL_HANDLE);
  _renderer->_renderPass->CreateFrameBuffers(sl->getService<ISwapchain>(),
                                             screen_width, screen_height);
}


Application::~Application() {
  glfwDestroyWindow(_window);
  glfwTerminate();
}

glm::vec2 convertToNDC(float mouseX, float mouseY, float windowWidth,
                       float windowHeight) {
  // Convert from [0, windowWidth] to [-1, 1] for X
  float ndcX = (mouseX / windowWidth) * 2.0f - 1.0f;
  // Convert from [0, windowHeight] to [-1, 1] for Y, but flip Y because
  // Vulkan's Y is inverted
  float ndcY = 1.0f - (mouseY / windowHeight) * 2.0f;
  return glm::vec2(ndcX, ndcY);
}

glm::vec2 convertMouseToWorld(float mouseX, float mouseY, float windowWidth,
                              float windowHeight) {
  glm::mat4 correction(
    glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, -1.0f, 0.0f, 0.0f),
    glm::vec4(0.0f, 0.0f, 0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 0.5f, 1.0f));

  auto PPM = 100.0f;

  float left = -(float) windowWidth / (2.0f * PPM);
  float right = (float) windowWidth / (2.0f * PPM);
  float bottom = -(float) windowHeight / (2.0f * PPM);
  float top = (float) windowHeight / (2.0f * PPM);
  auto perspective = glm::ortho(0.0f, (float) windowWidth / PPM,
                                (float) windowHeight / PPM, 0.0f, 0.1f, 256.0f);
  // Step 1: Convert mouse coordinates to NDC
  glm::vec2 ndc = convertToNDC(mouseX, mouseY, windowWidth, windowHeight);

  // Step 2: Combine projection and correction matrices
  glm::mat4 viewProjectionMatrix = correction * perspective;

  // Step 3: Invert the view-projection matrix
  glm::mat4 invViewProj = glm::inverse(viewProjectionMatrix);

  // Step 4: Convert NDC to world space
  glm::vec4 mouseNDC = glm::vec4(ndc.x, ndc.y, 0.0f, 1.0f);
  glm::vec4 mouseWorld = invViewProj * mouseNDC;

  // Step 5: Perform perspective divide (if necessary)
  mouseWorld /= mouseWorld.w;

  // Return the world space coordinates (x, y)
  return glm::vec2(mouseWorld.x, mouseWorld.y);
}

glm::vec2 convertWorldToScreen(glm::vec2 worldCoords, int width, int height) {
  glm::mat4 perspective = glm::ortho(
    0.0f, (float) width / 100.0f, (float) height / 100.0f, 0.0f, 0.1f, 256.0f);
  // Convert world coordinates to homogeneous clip space
  glm::vec4 clipSpaceCoords = perspective * glm::vec4(worldCoords, 0.0f, 1.0f);

  // Perform the perspective divide if necessary
  if (clipSpaceCoords.w != 0) {
    clipSpaceCoords /= clipSpaceCoords.w;
  }

  // Convert from clip space to normalized device coordinates
  float ndcX = (clipSpaceCoords.x + 1.0f) * 0.5f * width;
  float ndcY =
      (1.0f - (clipSpaceCoords.y + 1.0f) * 0.5f) * height; // Y is inverted

  return glm::vec2(ndcX, ndcY);
}

struct RGBA8 {
  uint8_t r, g, b, a;
};

RGBA8 MakeRGBA8(b2HexColor c, float alpha) {
  return {uint8_t((c >> 16) & 0xFF), uint8_t((c >> 8) & 0xFF), uint8_t(c & 0xFF), uint8_t(0xFF * alpha)};
}

/// Draw a solid closed polygon provided in CCW order.
void DrawSolidPolygon(b2Transform transform, const b2Vec2 *vertices, int vertexCount, float radius,
                      b2HexColor color,
                      void *context) {
  std::vector<Vertex> entropyVertices;
  for (int i = 0; i < vertexCount; i++) {
    Vertex vertex{};
    vertex.position.x = vertices[i].x;
    vertex.position.y = vertices[i].y;
    entropyVertices.push_back(vertex);
  }
  if (const auto app = static_cast<Application *>(context); app != nullptr) {
    app->vertices = entropyVertices;
  }

  /*
  const auto poly = PrimitiveFactory::CreateQuadFromVertices(entropyVertices);
  if (const auto position = poly.get_mut<Position>(); position != nullptr) {
    position->pos = glm::vec3(transform.p.x / 100.0f, transform.p.y / 100.0f, 0.0f);
  }
  if (const auto app = static_cast<Application *>(context); app != nullptr) {
    app->entities.push_back(poly);
  }
  */
}

void Application::Run() {
  this->OnInit();

  _timer->Start();
  _lastTick = _timer->GetTick();

  const ServiceLocator *sl = ServiceLocator::GetInstance();
  const auto world = sl->getService<IWorld>();
  const auto _lua = sl->getService<ILua>();
  const auto physics2d = sl->getService<IPhysics2D>();
  const auto debug2DDrawer = new b2DebugDraw();
  debug2DDrawer->drawShapes = true;
  debug2DDrawer->DrawSolidPolygon = DrawSolidPolygon;
  debug2DDrawer->context = this;


  std::vector<flecs::entity> lines;
  std::vector<flecs::entity> grid;

  while (!glfwWindowShouldClose(_window)) {
    // Wait for events when minimized
    while (isMinimized) {
      glfwWaitEvents();
    }

    // Calculate delta time
    _deltaTime = _timer->GetTick() - _lastTick;
    _lastTick = _timer->GetTick();

    _renderer->Render(screen_width, screen_height, needResize);
    this->OnRender(screen_width, screen_height, _deltaTime);

    auto on_render = _lua->Get()->get<sol::function>("OnRender");
    auto on_input = _lua->Get()->get<sol::function>("OnInput");

    if (on_render.valid()) {
      on_render(screen_width, screen_height);
    }

    if (on_input.valid()) {
      //on_input(mouse_x, mouse_y, mouse0_state);
    }

    glfwPollEvents();
    if (!vertices.empty()) {
      const auto poly = PrimitiveFactory::CreateQuadFromVertices(vertices);
      if (const auto position = poly.get_mut<Position>(); position != nullptr) {
        position->pos = glm::vec3(100 / 100.0f, 100 / 100.0f, 0.0f);
      }
    }


    constexpr float timeStep = 1.0f / 60.0f;
    constexpr int subStepCount = 4;
    b2World_Step(physics2d->Get(), timeStep, subStepCount);
    b2World_Draw(physics2d->Get(), debug2DDrawer);

    //
    //  this->_renderer->Render(width * xscale, height * yscale, xscale,
    //  yscale);
    //   On render

    // Increment current frame

    // if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    //   glfwSetWindowShouldClose(_window, true);

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

void character_callback(GLFWwindow *window, unsigned int codepoint) {
}

void cursor_position_callback(GLFWwindow *window, double xposIn,
                              double yposIn) {
  auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

  if (app == nullptr) {
    return;
  }

  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  /*

  app->mouse_x = xpos;
  app->mouse_y = ypos;

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

  */

  // app->_camera->ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_resize_callback(GLFWwindow *window, int width, int height) {
  auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
  if (app != nullptr) {
    app->needResize = true;

    // Update current resolution
    app->screen_width = width;
    app->screen_height = height;
  }
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

  /*
if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  app->mouse0_state = true;
if (button == GLFW_MOUSE_BUTTON_LEFT && action != GLFW_PRESS)
  app->mouse0_state = false;
  */

  // if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
  //   io.MouseDown[2] = true;
  // if (button == GLFW_MOUSE_BUTTON_RIGHT && action != GLFW_PRESS)
  //   io.MouseDown[2] = false;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
  // app->_camera->ProcessMouseScroll((float)yoffset);
}

void window_iconify_callback(GLFWwindow *window, int iconified) {
  auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

  if (iconified) {
    //app->isMinimized = true;
  } else {
    //app->isMinimized = false;
  }
}

void window_content_scale_callback(GLFWwindow *window, float xscale,
                                   float yscale) {
  auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

  app->xscale = xscale;
  app->yscale = yscale;
}

#endif
