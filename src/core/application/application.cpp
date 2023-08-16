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

    // Create Vulkan context
    auto context = Context();
    _context = std::make_shared<Context>(context);
}

/**
 * @brief
 *
 * @return int
 */
void Application::Run()
{
}