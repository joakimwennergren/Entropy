#pragma once

#include "context.hpp"
#include "renderpass.hpp"
#include "default_pipeline.hpp"
#include "commandbuffer.hpp"
#include "config.hpp"

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS)
#include <GLFW/glfw3.h>
#endif

#ifdef BUILD_FOR_IOS

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
#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS)
    GLFWwindow *_window;
#endif

    Symbios::Core::Context *_context;
    Symbios::Graphics::RenderPasses::Default *_renderPass;
    Symbios::Graphics::Pipeline::Default *_pipeline;
    Symbios::Graphics::CommandBuffers::CommandBuffer *_commandBuffer;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

private:
#ifdef BUILD_FOR_IOS
    NS::AutoreleasePool *_autoreleasePool;
#endif
};
