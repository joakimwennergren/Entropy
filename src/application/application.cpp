#include "application.hpp"

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX) || defined(BUILD_FOR_WINDOWS)
Application::Application()
{
    // Seed random
    srand(static_cast<unsigned>(time(0)));

    // Initialize GLFW
    if (!glfwInit())
    {
        PLOG_FATAL << "Could not initialize GLFW!";
        exit(EXIT_FAILURE);
    }

    // Create the window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    _window = glfwCreateWindow(640, 340, "Entropy application", NULL, NULL);

    if (!_window)
    {
        PLOG_FATAL << "Could not create the window!";
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
    screen.width = width;
    screen.height = height;

    VkExtent2D frame = {
        .width = (uint32_t)width,
        .height = (uint32_t)height};

    // Initialize Vulkan context
    VulkanContext::GetInstance()->Initialize(frame, _window);

    // Create the renderer
    _renderer = std::make_shared<Renderer>();

    // Create 1ms Timer
    _timer = new Timer(1.0f);
}

Application::~Application()
{
    delete _timer;
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Application::Run()
{
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
        screen.width = width;
        screen.height = height;

        this->_renderer->Render();
        this->OnRender(_deltaTime);

        // Poll events
        glfwPollEvents();
    }
}

void framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
    {
        app->GetRenderer()->Render();
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

#if defined(BUILD_FOR_ANDROID)

Application::Application()
{

    // Seed random
    srand(static_cast<unsigned>(time(0)));

    VkExtent2D frame = {
        .width = (uint32_t)500,
        .height = (uint32_t)1000};

    // Initialize Vulkan context
    // VulkanContext::GetInstance()->Initialize(window);

    // Create the renderer
    //_renderer = std::make_shared<Renderer>();

    // Create 1ms Timer
    _timer = new Timer(1.0f);
}

Application::~Application()
{
    delete _timer;
}

void Application::Run()
{
    this->OnInit();

    _timer->start();
    _lastTick = (float)_timer->get_tick();
}
#endif