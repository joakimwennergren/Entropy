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
    _window = glfwCreateWindow(640, 340, "Entropy application", NULL, NULL);

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
    VkExtent2D frame = {
        .width = (uint32_t)width,
        .height = (uint32_t)height};

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

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding textureLayoutBinding{};
    textureLayoutBinding.binding = 2;
    textureLayoutBinding.descriptorCount = 1;
    textureLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    textureLayoutBinding.pImmutableSamplers = nullptr;
    textureLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::vector<VkDescriptorSetLayoutBinding> bindings = {uboLayoutBinding, samplerLayoutBinding, textureLayoutBinding};

    auto descriptorSetLayout = std::make_shared<DescriptorsetLayout>(logicalDevice, bindings);
    auto descriptorSet = std::make_shared<Descriptorset>(logicalDevice, descriptorPool, descriptorSetLayout);

    // Add services to service locator
    serviceLocator = std::make_shared<ServiceLocator>();
    serviceLocator->registerService("PhysicalDevice", physicalDevice);
    serviceLocator->registerService("LogicalDevice", logicalDevice);
    serviceLocator->registerService("DescriptorSet", descriptorSet);
    serviceLocator->registerService("DescriptorSetLayout", descriptorSetLayout);
    serviceLocator->registerService("DescriptorPool", descriptorPool);
    serviceLocator->registerService("SwapChain", swapChain);
    serviceLocator->registerService("CommandPool", commandPool);

    sceneGraph = std::make_shared<SceneGraph>();
    serviceLocator->registerService("SceneGraph", sceneGraph);

    physics2d = std::make_shared<Physics2D>(serviceLocator);
    serviceLocator->registerService("2DPhysics", physics2d);

    lua = std::make_shared<Lua>(serviceLocator);

    _renderer = std::make_shared<Renderer>(serviceLocator);

    // Store it into a variable first, then call
    luaOnRender = lua->lua["OnRender"];
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

        // On render
        this->_renderer->Render();
        luaOnRender();
        this->OnRender(_deltaTime);
        float timeStep = 1.0f / 60.0f;
        int32 velocityIterations = 6;
        int32 positionIterations = 2;

        physics2d->world->Step(timeStep, velocityIterations, positionIterations);

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