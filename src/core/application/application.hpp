#pragma once

#include "context.hpp"
#include "config.h"

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <GLFW/glfw3.h>

// define something for Windows (32-bit and 64-bit, this part is common)
#ifdef _WIN64
// define something for Windows (64-bit only)
#else
// define something for Windows (32-bit only)
#endif
#elif __APPLE__

#include <GLFW/glfw3.h>

#if TARGET_IPHONE_SIMULATOR

#elif __ANDROID__
// Below __linux__ check should be enough to handle Android,
// but something may be unique to Android.
#elif __linux__
// linux
#elif __unix__ // all unices not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#error "Unknown compiler"
#endif
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
    GLFWwindow *_window;
    std::shared_ptr<Context> _context;
};
