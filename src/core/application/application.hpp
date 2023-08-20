#pragma once

#include "context.hpp"
#include "renderpass.hpp"
#include "default_pipeline.hpp"
#include "default_commandbuffer.hpp"
#include "config.h"

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#if BUILD_FOR_MACOS == true || BUILD_FOR_WINDOWS == true || BUILD_FOR_LINUX == true
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
#if BUILD_FOR_MACOS == true || BUILD_FOR_WINDOWS == true || BUILD_FOR_LINUX == true
    GLFWwindow *_window;
#endif

#if BUILD_FOR_IOS == true

#endif

    Symbios::Core::Context* _context;
    Symbios::Graphics::RenderPasses::Default *_renderPass;
    Symbios::Graphics::Pipeline::Default *_pipeline;
    Symbios::Graphics::CommandBuffers::Default *_commandBuffer;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;



};
