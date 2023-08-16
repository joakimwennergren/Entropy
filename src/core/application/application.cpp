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
    auto context = Context();
    _context = std::make_shared<Context>(context);
}

/**
 * @brief Destroy the Application:: Application object
 *
 */
Application::~Application()
{
    glfwTerminate();
}

/**
 * @brief
 *
 * @return int
 */
void Application::Run()
{
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(_window))
    {
        /* Poll for and process events */
        glfwPollEvents();
    }
}