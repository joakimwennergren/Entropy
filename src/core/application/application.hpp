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
    MTKViewDelegate(MTL::Device *pDevice) : MTK::ViewDelegate()
    {
    }
    virtual ~MTKViewDelegate() override
    {
        
    }
    inline virtual void drawInMTKView(MTK::View *pView) override
    {
        _renderer->Render();
    }
    Symbios::Graphics::Renderers::Renderer *_renderer;

private:
};

class Application : public UI::ApplicationDelegate
{
public:
    Application()
    {
        // Initialize logger
        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
        plog::init(plog::verbose, &consoleAppender);
        
        this->_autoreleasePool = NS::AutoreleasePool::alloc()->init();

        UI::ApplicationMain(0, 0, this);
    }
    ~Application()
    {
        _pMtkView->release();
        _pWindow->release();
        _pViewController->release();
        _pDevice->release();
        delete _pViewDelegate;
        this->_autoreleasePool->release();
    }

    inline bool applicationDidFinishLaunching(UI::Application *pApp, NS::Value *options) override
    {
        CGRect frame = UI::Screen::mainScreen()->bounds();

        _pWindow = UI::Window::alloc()->init(frame);

        _pViewController = UI::ViewController::alloc()->init(nil, nil);

        _pDevice = MTL::CreateSystemDefaultDevice();

        _pMtkView = MTK::View::alloc()->init(frame, _pDevice);
        _pMtkView->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
        _pMtkView->setClearColor(MTL::ClearColor::Make(1.0, 1.0, 1.0, 1.0));

        _pViewDelegate = new MTKViewDelegate(_pDevice);
        _pMtkView->setDelegate(_pViewDelegate);

        UI::View *mtkView = (UI::View *)_pMtkView;
        mtkView->setAutoresizingMask(UI::ViewAutoresizingFlexibleWidth | UI::ViewAutoresizingFlexibleHeight);
        _pViewController->view()->addSubview(mtkView);
        _pWindow->setRootViewController(_pViewController);

        _pWindow->makeKeyAndVisible();

        CA::MetalLayer *layer;

        layer = _pMtkView->currentDrawable()->layer();

        auto context = new Symbios::Core::Context(layer, frame);

        auto _renderer = new Symbios::Graphics::Renderers::Renderer(context);

        _pViewDelegate->_renderer = _renderer;

        return true;
    }
    inline void applicationWillTerminate(UI::Application *pApp) override
    {
    }

public:
    // virtual void Initialize() = 0;
    inline void Run()
    {
    }

private:
    Symbios::Core::Context *_context;

private:
    UI::Window *_pWindow;
    UI::ViewController *_pViewController;
    MTK::View *_pMtkView;
    MTL::Device *_pDevice;
    MTKViewDelegate *_pViewDelegate = nullptr;
    NS::AutoreleasePool *_autoreleasePool;
};

#endif
