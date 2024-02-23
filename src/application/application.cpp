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

    // Get initial window framebuffer size
    int width,
        height;
    glfwGetFramebufferSize(_window, &width, &height);
    VkExtent2D frame = {(uint32_t)width, (uint32_t)height};

    // Create 1ms Timer
    _timer = new Timer(1.0f);

    // Create items for vulkan
    _vkInstance = std::make_shared<VulkanInstance>("Entropy tests");
    _windowSurface = std::make_shared<WindowSurface>(_vkInstance, _window);
    _physicalDevice = std::make_shared<PhysicalDevice>(_vkInstance, _windowSurface);
    _logicalDevice = std::make_shared<LogicalDevice>(_physicalDevice, _windowSurface);
    _swapChain = std::make_shared<Swapchain>(_physicalDevice->Get(), _logicalDevice->Get(), _windowSurface, frame);
    auto commandPool = std::make_shared<CommandPool>(_logicalDevice, _physicalDevice, _windowSurface);
    auto descriptorPool = std::make_shared<DescriptorPool>(_logicalDevice);

    // Add services to service locator
    serviceLocator = std::make_shared<ServiceLocator>();

    _keyboard = std::make_shared<Keyboard>(serviceLocator);
    _camera = std::make_shared<Cam>(glm::vec3(0.0f, 0.0f, 3.0f));
    serviceLocator->AddService(_camera);
    serviceLocator->AddService(_keyboard);
    serviceLocator->AddService(_physicalDevice);
    serviceLocator->AddService(_logicalDevice);
    serviceLocator->AddService(descriptorPool);
    serviceLocator->AddService(_swapChain);
    serviceLocator->AddService(commandPool);
    serviceLocator->AddService(_windowSurface);

    sceneGraph = std::make_shared<SceneGraph>();
    serviceLocator->AddService(sceneGraph);

    physics2d = std::make_shared<Physics2D>(serviceLocator);
    serviceLocator->AddService(physics2d);

    mouse = std::make_shared<Mouse>(_window);
    serviceLocator->AddService(mouse);

    lua = std::make_shared<Lua>(serviceLocator);
    serviceLocator->AddService(lua);

    _renderer = std::make_shared<Renderer>(serviceLocator);

    io = ImGui::GetIO();

    // glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::ExecuteScripts(std::shared_ptr<SceneGraph> sceneGraph, std::shared_ptr<Lua> lua)
{
    for (auto renderable : sceneGraph->renderables)
    {
        if (!renderable->script->hasExecuted)
        {
            if (renderable->script->scriptFile.length() > 0)
            {
                lua->ExecuteScript("", renderable->script->scriptFile, renderable->script->environment);
            }
            else
            {
                lua->ExecuteScript(renderable->script->script, "", renderable->script->environment);
            }

            renderable->script->hasExecuted = true;
        }

        auto onRenderFunc = renderable->script->environment["OnRender"];

        // if (onRenderFunc != nullptr)
        //{
        //     onRenderFunc();
        // }
    }
}

Application::~Application()
{
    delete _timer;
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Application::Run()
{
    auto sceneGraph = serviceLocator->GetService<SceneGraph>();
    auto lua = serviceLocator->GetService<Lua>();

    this->OnInit();

    _timer->start();
    _lastTick = (float)_timer->get_tick();

    while (!glfwWindowShouldClose(_window))
    {
        // Calculate delta time
        _deltaTime = (float)_timer->get_tick() - _lastTick;
        _lastTick = (float)_timer->get_tick();

        auto &io = ImGui::GetIO();

        // Update screen dimensions
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);

        io.DisplaySize = ImVec2(width, height);
        io.MousePos.x = mouse_x;
        io.MousePos.y = mouse_y;
        io.MouseDown[0] = mouse0_state;

        this->OnRender(_deltaTime);
        // On render
        this->_renderer->Render(width, height, true);

        // Increment current frame

        if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(_window, true);

        if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
            _camera->ProcessKeyboard(FORWARD, 0.1);
        if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
            _camera->ProcessKeyboard(BACKWARD, 0.1);
        if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
            _camera->ProcessKeyboard(LEFT, 0.1);
        if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
            _camera->ProcessKeyboard(RIGHT, 0.1);

        float timeStep = 1.0f / 60.0f;
        // int32 velocityIterations = 6;
        // int32 positionIterations = 2;

        // physics2d->world->Step(timeStep, velocityIterations, positionIterations);

        // ExecuteScripts(sceneGraph, lua);

        // Poll events
        glfwPollEvents();
    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
}

void cursor_position_callback(GLFWwindow *window, double xposIn, double yposIn)
{

    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

    if (app == nullptr)
    {
        return;
    }

    app->mouse_x = (float)xposIn;
    app->mouse_y = (float)yposIn;

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

    app->_camera->ProcessMouseMovement(xoffset, yoffset);
}

void framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    // app->GetRenderer()->Render(width, height, true);
    app->OnRender(0.0);
    app->screen_width = width;
    app->screen_height = height;
    // app->GetRenderer()->HandleResize();
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        app->mouse0_state = true;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action != GLFW_PRESS)
        app->mouse0_state = false;
}

#endif
