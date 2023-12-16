/**
    application.hpp
    Application wrapper interface, common for all platforms
    @author Joakim Wennergren
    @version 1.0 2/11/2023
*/
#pragma once

#include <thread>
#include "stb_image.h"

#include <config.hpp>

#include <graphics/renderers/renderer.hpp>
#include <servicelocators/servicelocator.hpp>
#include <timing/timer.hpp>
#include <graphics/devices/logical_device.hpp>
#include "screen.hpp"

// new includes
#include <graphics/instances/vk_instance.hpp>
#include <graphics/surfaces/surface.hpp>
#include <graphics/devices/physical_device.hpp>
#include <graphics/swapchains/swapchain.hpp>
#include <graphics/imageviews/imageview.hpp>
#include <graphics/descriptorpools/descriptorpool.hpp>
#include <graphics/descriptorsetlayouts/descriptorsetlayout.hpp>
#include <graphics/descriptorsets/descriptorset.hpp>
#include <graphics/commandpools/commandpool.hpp>
#include <scenegraphs/scenegraph.hpp>

using namespace Entropy::Graphics::Instances;
using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::Swapchains;
using namespace Entropy::Graphics::ImageViews;
using namespace Entropy::Graphics::CommandPools;
using namespace Entropy::Graphics::DescriptorPools;
using namespace Entropy::Graphics::DescriptorsetLayouts;
using namespace Entropy::Graphics::Descriptorsets;
using namespace Entropy::SceneGraphs;

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)

#include <GLFW/glfw3.h>

// @todo remove symbios namespace
using namespace Symbios;
using namespace Entropy::Global;
using namespace Entropy::Graphics::Renderers;
using namespace Entropy::Timing;
using namespace Entropy::ServiceLocators;

void framebufferResizeCallback(GLFWwindow *window, int width, int height);
void cursorPositionCallback(GLFWwindow *window, double x, double y);

class Application
{
public:
    Application();
    ~Application();

    virtual void OnInit() = 0;

    virtual void OnRender(float deltaTime) = 0;

    void Run();

    // @todo make this more generic
    inline int GetScreenWidth() { return this->screen.width; };

    // @todo this shouldn't be public
    inline std::shared_ptr<Renderer> GetRenderer() { return this->_renderer; };

    // @todo look over if this should be protected..
protected:
    Screen screen;
    GLFWwindow *_window;
    std::shared_ptr<ServiceLocator> serviceLocator;
    std::shared_ptr<SceneGraph> sceneGraph;

private:
    std::shared_ptr<Renderer> _renderer;
    Timer *_timer;
    float _lastTick = 0.0f;
    float _deltaTime = 0.0f;
};
#endif

#if defined(BUILD_FOR_ANDROID)

// @todo remove symbios namespace
using namespace Symbios;
using namespace Entropy::Global;
using namespace Entropy::Graphics::Renderers;
using namespace Entropy::Timing;

class Application
{
public:
    Application();
    ~Application();

    virtual void OnInit() = 0;

    virtual void OnRender(float deltaTime) = 0;

    void Run();

    // @todo make this more generic
    inline int GetScreenWidth() { return this->screen.width; };

    // @todo this shouldn't be public
    inline std::shared_ptr<Renderer> GetRenderer() { return this->_renderer; };

    std::shared_ptr<Renderer> _renderer;

    // @todo look over if this should be protected..
protected:
    Screen screen;

private:
    Timer *_timer;
    float _lastTick = 0.0f;
    float _deltaTime = 0.0f;
};

#endif

#ifdef BUILD_FOR_IOS

#include <chrono>

#include <Metal/Metal.hpp>
#include <UIKit/UIKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include "filesystem.hpp"
#include "quad.hpp"
#include "sprite.hpp"
#include "label.hpp"
#include "easing.hpp"

using namespace Symbios;
using namespace Symbios::Text;
using namespace Symbios::Animation;

using namespace std::chrono;

extern "C" UI::ViewController *get_native_bounds(UI::View *view, UI::Screen *screen);
extern "C" CGPoint touch();

#include "renderer.hpp"

using namespace Symbios::Graphics::Renderers;

class Application : public UI::ApplicationDelegate
{
public:
    class MTKViewDelegate : public MTK::ViewDelegate
    {
    public:
        std::shared_ptr<SceneGraph> graph;
        /**
         * @brief Construct a new MTKViewDelegate object
         *
         */
        MTKViewDelegate(Application *app) : MTK::ViewDelegate()
        {
            this->app = app;
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
        Application *app;
        CGRect frame;

    private:
        float RandomFloat(float a, float b)
        {
            float random = ((float)rand()) / (float)RAND_MAX;
            float diff = b - a;
            float r = random * diff;
            return a + r;
        }

        double GetTimeAsDouble()
        {
            using namespace std::chrono;
            using SecondsFP = std::chrono::duration<double>;
            return duration_cast<SecondsFP>(high_resolution_clock::now().time_since_epoch()).count();
        }

        /**
         * @brief
         *
         * @param pView
         */
        inline virtual void drawInMTKView(MTK::View *pView) override
        {
            auto tick_time = (float)GetTimeAsDouble();

            deltaTime = tick_time - lastTick;

            lastTick = tick_time;

            app->OnRender(deltaTime);

            auto ratio = (app->frame.size.width) / (app->frame.size.height);

            app->screen.width = app->frame.size.width * 3.0;
            app->screen.height = app->frame.size.height * 3.3;

            _renderer->Render();
        }

        std::shared_ptr<Renderer> _renderer;

        float lastTick = 0.0;
        float deltaTime = 0.0;
    };

    /**
     * @brief Construct a new Application object
     *
     */
    Application()
    {

        this->_autoreleasePool = NS::AutoreleasePool::alloc()->init();
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
        frame = UI::Screen::mainScreen()->bounds();

        //_pViewController = UI::ViewController::alloc()->init(nil, nil);

        _pWindow = UI::Window::alloc()->init(frame);

        _pDevice = MTL::CreateSystemDefaultDevice();

        _pMtkView = MTK::View::alloc()->init(frame, _pDevice);

        _pViewController = get_native_bounds((UI::View *)_pMtkView, UI::Screen::mainScreen());

        _pMtkView->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
        _pMtkView->setClearColor(MTL::ClearColor::Make(1.0, 1.0, 1.0, 1.0));

        _pViewDelegate = new MTKViewDelegate(this);
        _pMtkView->setDelegate(_pViewDelegate);

        UI::View *mtkView = (UI::View *)_pMtkView;
        mtkView->setAutoresizingMask(UI::ViewAutoresizingFlexibleWidth | UI::ViewAutoresizingFlexibleHeight);

        _pViewController->view()->addSubview(mtkView);
        _pWindow->setRootViewController(_pViewController);

        _pWindow->makeKeyAndVisible();

        CA::MetalLayer *layer = _pMtkView->currentDrawable()->layer();

        Global::VulkanContext::GetInstance()->InitializeContext(layer, frame);

        auto renderer = std::make_shared<Renderer>();

        _pViewDelegate->SetRenderer(renderer);
        _pViewDelegate->frame = frame;

        this->_context = Global::VulkanContext::GetInstance()->GetVulkanContext();

        OnInit();

        return true;
    }

    virtual void OnInit() = 0;

    virtual void OnRender(float deltaTime) = 0;

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
        UI::ApplicationMain(0, 0, this);
    }

    std::shared_ptr<Context> _context;
    CGRect frame;
    Screen screen;

protected:
private:
    UI::Window *_pWindow = nullptr;
    MTK::View *_pMtkView = nullptr;
    MTL::Device *_pDevice = nullptr;
    MTKViewDelegate *_pViewDelegate = nullptr;
    NS::AutoreleasePool *_autoreleasePool = nullptr;
    UI::ViewController *_pViewController = nullptr;
};

#endif
