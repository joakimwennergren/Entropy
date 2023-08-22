#pragma once

#include "config.hpp"
#include "context.hpp"
#include "renderer.hpp"

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)
#include <GLFW/glfw3.h>
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
    Symbios::Core::Context *_context;
    Symbios::Graphics::Renderers::Renderer *_renderer;

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)
    GLFWwindow *_window;
#endif

private:
#ifdef BUILD_FOR_IOS
    NS::AutoreleasePool *_autoreleasePool;
#endif
};
