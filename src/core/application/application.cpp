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

#if BUILD_FOR_MACOS == true || BUILD_FOR_WINDOWS == true || BUILD_FOR_LINUX == true
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

    // Create Vulkan context
    auto context = new Symbios::Core::Context(_window);
    _context = std::make_shared<Symbios::Core::Context *>(context);

#endif
}

/**
 * @brief Destroy the Application:: Application object
 *
 */
Application::~Application()
{
#if BUILD_FOR_MACOS == true || BUILD_FOR_WINDOWS == true || BUILD_FOR_LINUX == true
    glfwDestroyWindow(_window);
    glfwTerminate();
#endif
}

/**
 * @brief
 *
 * @return int
 */
void Application::Run()
{
#if BUILD_FOR_MACOS == true || BUILD_FOR_WINDOWS == true || BUILD_FOR_LINUX == true
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(_window))
    {
        /* Poll for and process events */
        glfwPollEvents();
    }
#endif
}
