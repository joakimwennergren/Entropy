

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)
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

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Create a windowed mode window
    _window = glfwCreateWindow(640, 480, "Symbios dev application", NULL, NULL);

    if (!_window)
    {
        PLOG_FATAL << "Could not create window!";
        glfwTerminate();
        return;
    }

    this->_autoreleasePool = NS::AutoreleasePool::alloc()->init();

    _context = new Symbios::Core::Context(_window);
    _renderer = new Symbios::Graphics::Renderers::Renderer(_context);
}

/**
 * @brief Destroy the Application:: Application object
 *
 */
Application::~Application()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

/**
 * @brief
 *
 * @return int
 */
void Application::Run()
{
    while (!glfwWindowShouldClose(_window))
    {
        _renderer->Render();

        glfwPollEvents();
    }
}
#endif
