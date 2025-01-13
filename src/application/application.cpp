#include "application.hpp"
#include <ecs/iworld.hpp>
#include <graphics/renderers/vulkan_renderer.hpp>
#include <graphics/primitives/primitives.hpp>

using namespace Entropy::Graphics::Vulkan::Instances;
using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::SwapChains;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::CommandPools;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Graphics::Vulkan::DescriptorSetLayouts;
using namespace Entropy::Graphics::Vulkan::DescriptorSets;
using namespace Entropy::Graphics::Vulkan::Caches;
using namespace Entropy::Cameras;
using namespace Entropy::Timing;
using namespace Entropy::Scripting;
using namespace Entropy::Physics;
using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::ECS;
using namespace Entropy::Input;
using namespace Entropy::EntryPoints;

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX) || \
    defined(BUILD_FOR_WINDOWS)

Application::Application() {
  // Seed random
  // std::srand(std::time(0));

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
  glfwSetFramebufferSizeCallback(_window, OnFramebufferResize);
  glfwSetMouseButtonCallback(_window, OnMouseButtonCallback);
  glfwSetKeyCallback(_window, OnKey);
  glfwSetCursorPosCallback(_window, OnCursorPosition);
  glfwSetCharCallback(_window, OnCharacter);
  glfwSetScrollCallback(_window, OnScroll);
  glfwSetWindowIconifyCallback(_window, OnWindowIconify);
  glfwSetWindowContentScaleCallback(_window, OnWindowContentScale);

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

  // Create the renderer
  _renderer = std::make_shared<Renderers::VulkanRenderer>();

  // Create 1ms Timer
  _timer = std::make_unique<Timer>(1.0f);

  // Input
  keyboard = std::make_shared<Keyboard>();
  mouse = std::make_shared<Mouse>();

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

// Helper function to generate a unique hash for the polygon
size_t GeneratePolygonHash(const b2Vec2 *vertices, int vertexCount) {
  size_t hash = 0;

  for (int i = 0; i < vertexCount; i++) {
    constexpr size_t prime = 31;
    // Combine x and y coordinates using bitwise operations and primes
    hash = hash * prime + std::hash<float>()(vertices[i].x);
    hash = hash * prime + std::hash<float>()(vertices[i].y);
  }

  return hash;
}

void DrawSolidPolygon(b2Transform transform, const b2Vec2 *vertices, int vertexCount, float radius,
                      b2HexColor color, void *context) {
  /*
  const auto pool = static_cast<std::vector<flecs::entity> *>(context);

  // Check if an entity for this polygon already exists
  flecs::entity polyEntity;
  if (app->entityMap.find(polygonKey) == app->entityMap.end()) {
    // Create a new entity for this polygon
    std::vector<Vertex> entropyVertices;
    for (int i = 0; i < vertexCount; i++) {
      Vertex vertex{};
      vertex.position.x = vertices[i].x;
      vertex.position.y = vertices[i].y;
      entropyVertices.push_back(vertex);
    }

    polyEntity = PrimitiveFactory::CreateQuadFromVertices(entropyVertices);

    app->entityMap[polygonKey] = polyEntity; // Store the entity in the map
  } else {
    // Retrieve the existing entity for this polygon
    polyEntity = app->entityMap[polygonKey];
  }

  // Update the entity's position (transform)
  if (auto position = polyEntity.get_mut<Position>()) {
    position->pos = glm::vec3(transform.p.x / 100.0f, transform.p.y / 100.0f, 0.0f);
  }
  */
}

void Application::Run() {
  OnInit();

  _timer->Start();
  _lastTick = _timer->GetTick();

  const ServiceLocator *sl = ServiceLocator::GetInstance();
  const auto world = sl->getService<IWorld>();
  const auto _lua = sl->getService<ILua>();
  const auto physics2d = sl->getService<IPhysics2D>();

  // const auto debug2DDrawer = new b2DebugDraw();
  // debug2DDrawer->drawShapes = true;
  // debug2DDrawer->DrawSolidPolygon = DrawSolidPolygon;
  // debug2DDrawer->context = &physics2d->debugDrawEntities;

  while (!glfwWindowShouldClose(_window)) {
    // Wait for events when minimized
    while (isMinimized) {
      glfwWaitEvents();
    }

    // Calculate delta time
    _deltaTime = _timer->GetTick() - _lastTick;
    _lastTick = _timer->GetTick();

    glfwGetWindowContentScale(_window, &xscale, &yscale);

    _renderer->Render(screen_width, screen_height, xscale, yscale);

    OnRender(screen_width, screen_height, _deltaTime);

    auto on_render = _lua->Get()->get<sol::function>("OnRender");
    auto on_input = _lua->Get()->get<sol::function>("OnInput");

    if (on_render.valid()) {
      on_render(_deltaTime, screen_width, screen_height);
    }

    if (on_input.valid()) {
      // on_input(mouse_x, mouse_y, mouse0_state);
      // on_input(keyDown);
    }

    // 2D physics
    constexpr float timeStep = 1.0f / 60.0f;
    constexpr int subStepCount = 4;
    b2World_Step(physics2d->Get(), timeStep, subStepCount);
    // b2World_Draw(physics2d->Get(), debug2DDrawer);

    // Poll window events
    glfwPollEvents();
  }
}

void OnKey(GLFWwindow *window, const int key, const int scancode, const int action,
           const int mods) {
  if (const auto app = static_cast<Application *>(glfwGetWindowUserPointer(window)); app != nullptr) {
    if (action == GLFW_PRESS) {
      app->keyboard->keysDown[key] = true;
    }

    if (action == GLFW_RELEASE) {
      app->keyboard->keysDown[key] = false;
    }

    app->keyboard->keyCtrl = app->keyboard->keysDown[GLFW_KEY_LEFT_CONTROL] || app->keyboard->keysDown[
                               GLFW_KEY_RIGHT_CONTROL];

    app->keyboard->keyShift = app->keyboard->keysDown[GLFW_KEY_LEFT_SHIFT] || app->keyboard->keysDown[
                                GLFW_KEY_RIGHT_SHIFT];

    app->keyboard->keyAlt = app->keyboard->keysDown[GLFW_KEY_LEFT_ALT] || app->keyboard->keysDown[GLFW_KEY_RIGHT_ALT];

    app->keyboard->keySuper = app->keyboard->keysDown[GLFW_KEY_LEFT_SUPER] || app->keyboard->keysDown[
                                GLFW_KEY_RIGHT_SUPER];
  }
}

void OnCharacter(GLFWwindow *window, unsigned int codepoint) {
}

void OnCursorPosition(GLFWwindow *window, const double xpos,
                      const double ypos) {
  if (const auto app = static_cast<Application *>(glfwGetWindowUserPointer(window)); app != nullptr) {
    app->mouse->pos = glm::vec2(xpos, ypos);
  }
}

void OnFramebufferResize(GLFWwindow *window, const int width, const int height) {
  if (const auto app = static_cast<Application *>(glfwGetWindowUserPointer(window)); app != nullptr) {
    app->screen_width = width;
    app->screen_height = height;
    app->GetVulkanRenderer()->OnResize(width, height);
    app->GetVulkanRenderer()->Render(width, height, 1.0, 1.0);
    const auto lua = ServiceLocator::GetInstance()->getService<ILua>();
    if (const auto on_render = lua->Get()->get<sol::function>("OnRender"); on_render.valid()) {
      on_render(1.0f, width, height);
    }
  }
}

void OnMouseButtonCallback(GLFWwindow *window, const int button, const int action,
                           int mods) {
  if (const auto app = static_cast<Application *>(glfwGetWindowUserPointer(window)); app != nullptr) {
    if (action == GLFW_PRESS) {
      app->mouse->mouseDown[button] = true;
    }
    if (action == GLFW_RELEASE) {
      app->mouse->mouseDown[button] = false;
    }
  }
}

void OnScroll(GLFWwindow *window, const double xoffset, const double yoffset) {
  if (const auto app = static_cast<Application *>(glfwGetWindowUserPointer(window)); app != nullptr) {
    app->mouse->scroll = glm::vec2(xoffset, yoffset);
  }
}

void OnWindowIconify(GLFWwindow *window, const int iconified) {
  if (const auto app = static_cast<Application *>(glfwGetWindowUserPointer(window)); app != nullptr) {
    if (iconified) {
      app->isMinimized = true;
    } else {
      app->isMinimized = false;
    }
  }
}

void OnWindowContentScale(GLFWwindow *window, const float xscale,
                          const float yscale) {
  if (const auto app = static_cast<Application *>(glfwGetWindowUserPointer(window)); app != nullptr) {
    app->xscale = xscale;
    app->yscale = yscale;
  }
}

#endif
