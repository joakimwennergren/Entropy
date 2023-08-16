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

    // Create Window Description
    xwin::WindowDesc windowDesc;
    windowDesc.name = "Test";
    windowDesc.title = "My Title";
    windowDesc.visible = true;
    windowDesc.width = 640;
    windowDesc.backgroundColor = 0xFF00CCFF;
    windowDesc.height = 480;

    if (!_window.create(windowDesc, _eventQueue))
    {
#if USE_LOGGING == true
        PLOG_FATAL << "Could not create 'CrossWindow' window!";
#endif
        return;
    }

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
    bool isRunning = true;

    while (isRunning)
    {
        // Update the event queue
        _eventQueue.update();

        // Process events
        while (!_eventQueue.empty())
        {
            const xwin::Event &event = _eventQueue.front();

            if (event.type == xwin::EventType::Close)
            {
                _window.close();
                isRunning = false;
            }

            _eventQueue.pop();
        }
    }
}