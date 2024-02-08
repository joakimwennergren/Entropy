#include "application.hpp"

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX) || defined(BUILD_FOR_WINDOWS)
Application::Application()
{
    // Seed random
    srand(static_cast<unsigned>(time(0)));

    // Initialize GLFW
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    // Create the window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    _window = glfwCreateWindow(640, 480, "Entropy application", NULL, NULL);

    if (!_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Bind window callbacks
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
    glfwSetCursorPosCallback(_window, cursorPositionCallback);

    // Get initial window framebuffer size
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);
    VkExtent2D frame = {(uint32_t)width, (uint32_t)height};

    // Create 1ms Timer
    _timer = new Timer(1.0f);

    // Create items for vulkan
    auto vkInstance = std::make_shared<VulkanInstance>("Entropy tests");
    auto windowSurface = std::make_shared<WindowSurface>(vkInstance, _window);
    auto physicalDevice = std::make_shared<PhysicalDevice>(vkInstance, windowSurface);
    auto logicalDevice = std::make_shared<LogicalDevice>(physicalDevice, windowSurface);
    auto swapChain = std::make_shared<Swapchain>(physicalDevice->Get(), logicalDevice->Get(), windowSurface, frame);
    auto commandPool = std::make_shared<CommandPool>(logicalDevice, physicalDevice, windowSurface);
    auto descriptorPool = std::make_shared<DescriptorPool>(logicalDevice);

    // Add services to service locator
    serviceLocator = std::make_shared<ServiceLocator>();
    serviceLocator->AddService(physicalDevice);
    serviceLocator->AddService(logicalDevice);
    serviceLocator->AddService(descriptorPool);
    serviceLocator->AddService(swapChain);
    serviceLocator->AddService(commandPool);

    sceneGraph = std::make_shared<SceneGraph>();
    serviceLocator->AddService(sceneGraph);

    physics2d = std::make_shared<Physics2D>(serviceLocator);
    serviceLocator->AddService(physics2d);

    lua = std::make_shared<Lua>(serviceLocator);
    serviceLocator->AddService(lua);

    _renderer = std::make_shared<Renderer>(serviceLocator);
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

        if (onRenderFunc.valid())
        {
            onRenderFunc();
        }
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

        // Update screen dimensions
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);
        // screen.width = width;
        // screen.height = height;

        // On render
        this->OnRender(_deltaTime);
        this->_renderer->Render(width, height);

        float timeStep = 1.0f / 60.0f;
        // int32 velocityIterations = 6;
        // int32 positionIterations = 2;

        // physics2d->world->Step(timeStep, velocityIterations, positionIterations);

        // ExecuteScripts(sceneGraph, lua);

        // Poll events
        glfwPollEvents();
    }
}

void framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
    {
        app->GetRenderer()->Render(width, height);
        app->GetRenderer()->Render(width, height);
        app->OnRender(0);
    }
}

void cursorPositionCallback(GLFWwindow *window, double x, double y)
{
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
    {
        // app->GetRenderer()->pane->position = glm::vec3((float)x, (float)y, 0.0);
    }
}
#endif
