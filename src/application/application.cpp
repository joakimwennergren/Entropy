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

    sceneGraph = std::make_shared<SceneGraph>();
    serviceLocator->AddService(sceneGraph);

    physics2d = std::make_shared<Physics2D>(serviceLocator);
    serviceLocator->AddService(physics2d);

    mouse = std::make_shared<Mouse>(_window);
    serviceLocator->AddService(mouse);

    lua = std::make_shared<Lua>(serviceLocator);
    serviceLocator->AddService(lua);

    GLFWmonitor *primary = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(primary, &xscale, &yscale);

    _renderer = std::make_shared<Renderer>(serviceLocator, &world, xscale, yscale);
    auto &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(width, height);

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;

    // Build the broadphase
    broadphase = new btDbvtBroadphase();

    // Set up the collision configuration and dispatcher
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    // The actual physics solver
    solver = new btSequentialImpulseConstraintSolver;

    // The world.
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

    boxCollisionShape = new btBoxShape(btVector3(10.0f, 10.0f, 10.0f));

    btDefaultMotionState *motionstate = new btDefaultMotionState(btTransform(
        btQuaternion(0.0, 0.0, 0.0, 0.0),
        btVector3(0.0, 0.0, 0.0)));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        0, // mass, in kg. 0 -> Static object, will never move.
        motionstate,
        boxCollisionShape, // collision shape of body
        btVector3(0, 0, 0) // local inertia
    );
    btRigidBody *rigidBody = new btRigidBody(rigidBodyCI);

    dynamicsWorld->addRigidBody(rigidBody);
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
    auto &io = ImGui::GetIO();

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

        this->OnRender(_deltaTime);
        this->_renderer->Render(width, height, true);
        // On render

        // Increment current frame

        if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(_window, true);

        if (io.MouseDown[2])
        {
            if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
                _camera->ProcessKeyboard(FORWARD, 0.1);
            if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
                _camera->ProcessKeyboard(BACKWARD, 0.1);
            if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
                _camera->ProcessKeyboard(LEFT, 0.1);
            if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
                _camera->ProcessKeyboard(RIGHT, 0.1);
        }

        double mx, my;
        glfwGetCursorPos(_window, &mx, &my);

        // The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
        glm::vec4 lRayStart_NDC(
            ((float)mx / (float)width - 0.5f) * 2.0f,  // [0,1024] -> [-1,1]
            ((float)my / (float)height - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
            -1.0,                                      // The near plane maps to Z=-1 in Normalized Device Coordinates
            1.0f);
        glm::vec4 lRayEnd_NDC(
            ((float)mx / (float)width - 0.5f) * 2.0f,
            ((float)my / (float)height - 0.5f) * 2.0f,
            0.0,
            1.0f);

        // Faster way (just one inverse)
        glm::mat4 M = glm::inverse(_renderer->_camera->matrices.perspective * _camera->GetViewMatrix());
        glm::vec4 lRayStart_world = M * lRayStart_NDC;
        lRayStart_world /= lRayStart_world.w;
        glm::vec4 lRayEnd_world = M * lRayEnd_NDC;
        lRayEnd_world /= lRayEnd_world.w;

        glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
        lRayDir_world = glm::normalize(lRayDir_world);

        glm::vec3 out_end = _camera->Position + _camera->Up * 1000.0f;

        btCollisionWorld::ClosestRayResultCallback RayCallback(
            btVector3(_camera->Position.x, _camera->Position.y, _camera->Position.z),
            btVector3(out_end.x, out_end.y, out_end.z));
        btVector3 from(0.0, 0.0, 0.0);
        btVector3 to(0.0, 0.0, 1.0);
        dynamicsWorld->rayTest(
            from,
            to,
            RayCallback);

        if (RayCallback.hasHit())
        {
            std::cout << "HIT = " << std::endl;
        }

        //     float timeStep = 1.0f / 60.0f;
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

    if (io.MouseDown[2])
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
    app->GetRenderer()->Render(width, height, true);
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
    ImGuiIO &io = ImGui::GetIO();
    io.MouseWheelH += (float)xoffset;
    io.MouseWheel += (float)yoffset;
}

#endif
