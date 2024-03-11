#include "application.hpp"

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX) || defined(BUILD_FOR_WINDOWS)
Application::Application()
{
    ImGui::CreateContext();
    // Seed random
    srand(static_cast<unsigned>(time(0)));

    // Initialize GLFW
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    // Create the window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    _window = glfwCreateWindow(500, 500, "Entropy application", NULL, NULL);

    if (!_window)
    {
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

    // Get initial window framebuffer size
    int width,
        height;
    glfwGetFramebufferSize(_window, &width, &height);
    VkExtent2D frame = {(uint32_t)width, (uint32_t)height};

    // Create 1ms Timer
    _timer = std::make_unique<Timer>(1.0);

    // Create items for vulkan
    _vkInstance = std::make_shared<VulkanInstance>("Entropy tests");
    _windowSurface = std::make_shared<WindowSurface>(_vkInstance, _window);
    _physicalDevice = std::make_shared<PhysicalDevice>(_vkInstance, _windowSurface);
    _logicalDevice = std::make_shared<LogicalDevice>(_physicalDevice, _windowSurface);
    _allocator = std::make_shared<Allocator>(_vkInstance, _logicalDevice, _physicalDevice);
    _swapChain = std::make_shared<Swapchain>(_physicalDevice->Get(), _logicalDevice->Get(), _windowSurface, frame);
    auto commandPool = std::make_shared<CommandPool>(_logicalDevice, _physicalDevice, _windowSurface);
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
    serviceLocator->AddService(_windowSurface);

    physics2d = std::make_shared<Physics2D>(serviceLocator);
    serviceLocator->AddService(physics2d);

    mouse = std::make_shared<Mouse>(_window);
    serviceLocator->AddService(mouse);

    lua = std::make_shared<Lua>(serviceLocator);
    serviceLocator->AddService(lua);

    world = std::make_shared<World>();
    serviceLocator->AddService(world);

    auto queueSync = std::make_shared<QueueSync>(serviceLocator);
    serviceLocator->AddService(queueSync);

    physics3d = std::make_shared<Physics3D>(&world->gameWorld, serviceLocator);
    serviceLocator->AddService(physics3d);

    GLFWmonitor *primary = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(primary, &xscale, &yscale);

    _renderer = std::make_shared<Renderer>(serviceLocator, &world->gameWorld, xscale, yscale);

    auto &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(width, height);

    _renderer->_camera->setPerspective(60.0f, (float)width / (float)height, 1.0f, 100000.0f);
}

// void Application::ExecuteScripts(std::shared_ptr<SceneGraph> sceneGraph, std::shared_ptr<Lua> lua)
// {
//     for (auto renderable : sceneGraph->renderables)
//     {
//         if (!renderable->script->hasExecuted)
//         {
//             if (renderable->script->scriptFile.length() > 0)
//             {
//                 lua->ExecuteScript("", renderable->script->scriptFile, renderable->script->environment);
//             }
//             else
//             {
//                 lua->ExecuteScript(renderable->script->script, "", renderable->script->environment);
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

Application::~Application()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Application::Run()
{
    auto lua = serviceLocator->GetService<Lua>();
    auto physics3d = serviceLocator->GetService<Physics3D>();
    auto &io = ImGui::GetIO();

    this->OnInit();

    _timer->Start();
    _lastTick = (float)_timer->GetTick();

    while (!glfwWindowShouldClose(_window))
    {

        // spdlog::info("ms/frame = {}", _deltaTime);

        // Calculate delta time
        _deltaTime = _timer->GetTick() - _lastTick;
        _lastTick = _timer->GetTick();

        // Update screen dimensions
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX) || defined(BUILD_FOR_WINDOWS)
        while (isMinimized)
        {
            glfwWaitEvents();
        }
#endif

        GLFWmonitor *primary = glfwGetPrimaryMonitor();
        float xscale, yscale;
        glfwGetMonitorContentScale(primary, &xscale, &yscale);

        this->OnRender(_deltaTime);
        this->_renderer->Render(width, height, true);
        // On render

        // Increment current frame

        if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(_window, true);

        if (io.MouseDown[2] || io.MouseDown[1])
        {
            float speed = 0.1;
            if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
            {
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

        //     float timeStep = 1.0f / 60.0f;
        // int32 velocityIterations = 6;
        // int32 positionIterations = 2;

        // physics2d->world->Step(timeStep, velocityIterations, positionIterations);
        physics3d->GetWorld()->stepSimulation(1.0f / 60.0f); // Example: step the simulation with a time step of 1/60 seconds

        // ExecuteScripts(sceneGraph, lua);

        _camera->updateCameraVectors();
        _renderer->_camera->setPerspective(_camera->Zoom, (float)width / (float)height, 1.0f, 100000.0f);

        uint32_t index = 0;
        for (auto fut : lua->futures)
        {
            if (fut.wait_for(1ms) == std::future_status::ready)
            {
                std::cout << "Building model!" << std::endl;
                auto e = serviceLocator->GetService<World>()->gameWorld.entity();
                auto id = AssetId().GetId();
                auto modelShared = std::shared_ptr<Entropy::GLTF::Model>(fut.get());
                e.set<Position>({glm::vec3(0.0, 0.0, 0.0)});
                e.set<Scale>({glm::vec3(100.0, 100.0, 100.0)});
                e.set<Entropy::Components::Model>({modelShared});
                e.set<Entropy::Components::Renderable>({id, 0, true});
                e.set<Entropy::Components::Color>({glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}});
                e.set<Entropy::Components::BoxCollisionShape3D>({glm::vec3(10.0, 10.0, 10.0), glm::vec3{0.0, 0.0, 0.0}});
                e.set<Entropy::Components::RigidBody3D>({});
                e.set<Entropy::Components::Scripted>({});
                e.get_mut<Entropy::Components::BoxCollisionShape3D>()->UpdateMotionState();
                e.get_mut<Entropy::Components::RigidBody3D>()->Initialize(serviceLocator, e);
                lua->loadedModels[index] = e;
                lua->futures.erase(lua->futures.begin() + index);
            }
            index++;
        }

        // Poll events
        glfwPollEvents();
    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    ImGuiIO &io = ImGui::GetIO();
    if (action == GLFW_PRESS)
        io.KeysDown[key] = true;
    if (action == GLFW_RELEASE)
        io.KeysDown[key] = false;
    // Modifiers are not reliable across systems
    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

void character_callback(GLFWwindow *window, unsigned int codepoint)
{
    ImGuiIO &io = ImGui::GetIO();

    io.AddInputCharacter(codepoint);
}

void cursor_position_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    auto &io = ImGui::GetIO();
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

    if (app == nullptr)
    {
        return;
    }

    GLFWmonitor *primary = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(primary, &xscale, &yscale);

    app->mouse_x = (float)xposIn * xscale;
    app->mouse_y = (float)yposIn * yscale;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (app->firstMouse)
    {
        app->lastX = xpos;
        app->lastY = ypos;
        app->firstMouse = false;
    }

    float xoffset = xpos - app->lastX;
    float yoffset = app->lastY - ypos; // reversed since y-coordinates go from bottom to top

    app->lastX = xpos;
    app->lastY = ypos;

    io.MousePos.x = (float)xposIn * xscale;
    io.MousePos.y = (float)yposIn * yscale;

    if (io.MouseDown[2] || io.MouseDown[1])
        app->_camera->ProcessMouseMovement(xoffset, yoffset);
}

void framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

    GLFWmonitor *primary = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(primary, &xscale, &yscale);

    auto &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(width * xscale, height * yscale);
    app->GetRenderer()->Render(width * xscale, height * yscale, true);
    app->_renderer->_camera->setPerspective(app->_camera->Zoom, (float)width / (float)height, 1.0f, 100000.0f);
    app->OnRender(0.0);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    auto &io = ImGui::GetIO();
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        io.MouseDown[0] = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action != GLFW_PRESS)
        io.MouseDown[0] = false;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        io.MouseDown[2] = true;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action != GLFW_PRESS)
        io.MouseDown[2] = false;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    ImGuiIO &io = ImGui::GetIO();
    io.MouseWheelH += (float)xoffset;
    io.MouseWheel += (float)yoffset;

    app->_camera->ProcessMouseScroll((float)yoffset);
}

void window_iconify_callback(GLFWwindow *window, int iconified)
{
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

    if (iconified)
    {
        app->isMinimized = true;
    }
    else
    {
        app->isMinimized = false;
    }
}

#endif
