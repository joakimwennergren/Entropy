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

    // Create Vulkan context
    auto context = new Symbios::Core::Context(nullptr);
    _context = std::make_shared<Symbios::Core::Context *>(context);
}

/**
 * @brief Destroy the Application:: Application object
 *
 */
Application::~Application()
{
#if BUILD_FOR_DESKTOP == true
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
#if BUILD_FOR_DESKTOP == true
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(_window))
    {
        /* Poll for and process events */
        glfwPollEvents();
    }
#endif
}
