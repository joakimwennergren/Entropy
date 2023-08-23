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

using namespace Symbios::Core;
using namespace Symbios::Graphics::Renderers;

static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

/**
 * @brief Application class
 *
 */
class Application
{
public:
    /**
     * @brief Construct a new Application object
     *
     */
    Application()
    {
        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
        plog::init(plog::verbose, &consoleAppender);

        if (!glfwInit())
        {
            PLOG_FATAL << "Could not initialize GLFW library!";
            exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        _window = glfwCreateWindow(640, 480, "Symbios dev application", NULL, NULL);

        if (!_window)
        {
            PLOG_FATAL << "Could not create window!";
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwSetWindowUserPointer(_window, this);

        glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);

        _context = std::make_shared<Context>(_window);
        _renderer = std::make_shared<Renderer>(_context);
    }

    /**
     * @brief Destroy the Application object
     *
     */
    ~Application()
    {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    std::shared_ptr<Renderer> GetRenderer() { return this->_renderer; };

public:
    /**
     * @brief
     *
     */
    inline void Run()
    {
        while (!glfwWindowShouldClose(_window))
        {
            glfwPollEvents();
            _renderer->Render();
        }
    }

private:
    std::shared_ptr<Context> _context;
    std::shared_ptr<Renderer> _renderer;
    GLFWwindow *_window;

private:
};

static void framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
    {
        app->GetRenderer()->FrameBufferResized();
    }
}

#endif

#ifdef BUILD_FOR_IOS

#include <Metal/Metal.hpp>
#include <UIKit/UIKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include "renderer.hpp"

using namespace Symbios::Graphics::Renderers;

class MTKViewDelegate : public MTK::ViewDelegate
{
public:
    /**
     * @brief Construct a new MTKViewDelegate object
     *
     */
    MTKViewDelegate() : MTK::ViewDelegate()
    {
    }

    inline void SetRenderer(std::shared_ptr<Renderer> renderer)
    {
        _renderer = renderer;
    }

    /**
     * @brief Destroy the MTKViewDelegate object
     *
     */
    virtual ~MTKViewDelegate() override
    {
    }

private:
    /**
     * @brief
     *
     * @param pView
     */
    inline virtual void drawInMTKView(MTK::View *pView) override
    {
        _renderer->Render();
    }

    std::shared_ptr<Renderer> _renderer;
};

class Application : public UI::ApplicationDelegate
{
public:
    /**
     * @brief Construct a new Application object
     *
     */
    Application()
    {
        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
        plog::init(plog::verbose, &consoleAppender);

        this->_autoreleasePool = NS::AutoreleasePool::alloc()->init();

        UI::ApplicationMain(0, 0, this);
    }

    /**
     * @brief Destroy the Application object
     *
     */
    ~Application()
    {
        _pMtkView->release();
        _pWindow->release();
        _pViewController->release();
        _pDevice->release();
        delete _pViewDelegate;
        this->_autoreleasePool->release();
    }

    /**
     * @brief
     *
     * @param pApp
     * @param options
     * @return true
     * @return false
     */
    inline bool applicationDidFinishLaunching(UI::Application *pApp, NS::Value *options) override
    {
        CGRect frame = UI::Screen::mainScreen()->bounds();

        _pWindow = UI::Window::alloc()->init(frame);

        _pViewController = UI::ViewController::alloc()->init(nil, nil);

        _pDevice = MTL::CreateSystemDefaultDevice();

        _pMtkView = MTK::View::alloc()->init(frame, _pDevice);
        _pMtkView->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
        _pMtkView->setClearColor(MTL::ClearColor::Make(1.0, 1.0, 1.0, 1.0));

        _pViewDelegate = new MTKViewDelegate();
        _pMtkView->setDelegate(_pViewDelegate);

        UI::View *mtkView = (UI::View *)_pMtkView;
        mtkView->setAutoresizingMask(UI::ViewAutoresizingFlexibleWidth | UI::ViewAutoresizingFlexibleHeight);
        _pViewController->view()->addSubview(mtkView);
        _pWindow->setRootViewController(_pViewController);

        _pWindow->makeKeyAndVisible();

        CA::MetalLayer *layer = _pMtkView->currentDrawable()->layer();

        _context = std::make_shared<Context>(layer, frame);

        auto renderer = std::make_shared<Renderer>(_context);

        _pViewDelegate->SetRenderer(renderer);

        return true;
    }

    /**
     * @brief
     *
     * @param pApp
     */
    inline void applicationWillTerminate(UI::Application *pApp) override
    {
    }

public:
    /**
     * @brief
     *
     */
    inline void Run()
    {
    }

private:
    std::shared_ptr<Context> _context;
    UI::Window *_pWindow = nullptr;
    UI::ViewController *_pViewController = nullptr;
    MTK::View *_pMtkView = nullptr;
    MTL::Device *_pDevice = nullptr;
    MTKViewDelegate *_pViewDelegate = nullptr;
    NS::AutoreleasePool *_autoreleasePool = nullptr;
};

#endif
