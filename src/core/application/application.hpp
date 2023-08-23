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

        _context = new Context(_window);
        _renderer = std::make_unique<Renderer>(_context);
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
    Context *_context;
    std::unique_ptr<Renderer> _renderer;
    GLFWwindow *_window;

private:
};

#endif

#ifdef BUILD_FOR_IOS

#include <Metal/Metal.hpp>
#include <UIKit/UIKit.hpp>
#include <MetalKit/MetalKit.hpp>

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

    Symbios::Graphics::Renderers::Renderer *_renderer;
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

        auto context = new Symbios::Core::Context(layer, frame);

        auto _renderer = new Symbios::Graphics::Renderers::Renderer(context);

        _pViewDelegate->_renderer = _renderer;

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
    Context *_context;
    UI::Window *_pWindow;
    UI::ViewController *_pViewController;
    MTK::View *_pMtkView;
    MTL::Device *_pDevice;
    MTKViewDelegate *_pViewDelegate = nullptr;
    NS::AutoreleasePool *_autoreleasePool;
};

#endif
