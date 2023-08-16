#include "application.hpp"

/**
 * @brief Construct a new Application:: Application object
 *
 */
Application::Application()
{
    // Initialize logger
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

#if BUILD_FOR_DESKTOP == true
    // Initialize GLFW
    if (!glfwInit())
    {
        PLOG_FATAL << "Could not initialize GLFW library!";
        return;
    }

    // Create a windowed mode window
    _window = glfwCreateWindow(640, 480, "Symbios dev application", NULL, NULL);

    if (!_window)
    {
        PLOG_FATAL << "Could not create window!";
        glfwTerminate();
        return;
    }
#endif

#if BUILD_FOR_IOS == true
    _pAutoreleasePool = NS::AutoreleasePool::alloc()->init();
#endif

    // Create Vulkan context
    auto context = Context();
    _context = std::make_shared<Context>(context);
}

/**
 * @brief Destroy the Application:: Application object
 *
 */
Application::~Application()
{
#if BUILD_FOR_DESKTOP == true
    glfwTerminate();
#endif

#if BUILD_FOR_IOS == true
    this->_pAutoreleasePool->release();
#endif
}

/**
 * @brief
 *
 * @return int
 */
void Application::Run()
{
#if BUILD_FOR_DESKTOP == true
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(_window))
    {
        /* Poll for and process events */
        glfwPollEvents();
    }
#endif

#if BUILD_FOR_IOS == true

    MyAppDelegate del;
    UI::ApplicationMain(0, 0, &del);

#endif
}