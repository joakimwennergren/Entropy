#pragma once

#include "context.hpp"
#include "config.h"

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#if BUILD_FOR_DESKTOP == true
#include <GLFW/glfw3.h>
#endif

#if BUILD_FOR_IOS == true
#endif

/**
 * @brief Application class
 *
 */
class Application
{
public:
    Application();
    ~Application();

public:
    // virtual void Initialize() = 0;
    void Run();

private:
#if BUILD_FOR_DESKTOP == true
    GLFWwindow *_window;
#endif
#if BUILD_FOR_IOS == true

#endif
    std::shared_ptr<Context>
        _context;
};
