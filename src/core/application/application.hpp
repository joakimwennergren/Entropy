#pragma once

#include "config.hpp"
#include "context.hpp"
#include "renderer.hpp"

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include "plog/Initializers/RollingFileInitializer.h"

struct Screen
{
    int width;
    int height;
};


#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)

#include <GLFW/glfw3.h>

using namespace Symbios;
using namespace Symbios::Core;
using namespace Symbios::Graphics::Renderers;

static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
static void cursorPositionCallback(GLFWwindow *window, double x, double y);

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
        // plog::init(plog::debug, "debug.txt"); // Step2: initialize the logger
        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
        plog::init(plog::verbose, &consoleAppender);

        srand(static_cast<unsigned>(time(0)));

        if (!glfwInit())
        {
            PLOG_FATAL << "Could not initialize GLFW library!";
            exit(EXIT_FAILURE);
        }
        // glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        _window = glfwCreateWindow(640, 340, "Symbios dev application", NULL, NULL);

        if (!_window)
        {
            PLOG_FATAL << "Could not create window!";
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwSetWindowUserPointer(_window, this);

        glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
        glfwSetCursorPosCallback(_window, cursorPositionCallback);


        Global::GetInstance()->InitializeContext(_window);
    
        _renderer = std::make_shared<Renderer>();

        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);

        screen.width = width;
        screen.height = height;
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

    inline int GetScreenWidth() {return this->screen.width;};

    virtual void OnInit() = 0;
    
    virtual void OnRender(float deltaTime) = 0;

    std::shared_ptr<Renderer> GetRenderer() { return this->_renderer; };

public:
    /**
     * @brief
     *
     */
    inline void Run()
    {

        this->OnInit();

        while (!glfwWindowShouldClose(_window))
        {
            int width, height;
            glfwGetFramebufferSize(_window, &width, &height);

            screen.width = width;
            screen.height = height;

            this->OnRender(0.0f);

            glfwPollEvents();
            _renderer->Render();
        }
    }

protected:
        std::shared_ptr<Context> _context;
        GLFWwindow *_window;
        Screen screen;
        chaiscript::ChaiScript * chai;
private:

    std::shared_ptr<Renderer> _renderer;


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

static void cursorPositionCallback(GLFWwindow *window, double x, double y)
{
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
    {
        // app->GetRenderer()->pane->position = glm::vec3((float)x, (float)y, 0.0);
    }
}

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
        
        float RandomFloat(float a, float b) {
            float random = ((float) rand()) / (float) RAND_MAX;
            float diff = b - a;
            float r = random * diff;
            return a + r;
        }
                
        double GetTimeAsDouble() {
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

            app->screen.width = app->frame.size.width * ratio * 0.95;
            app->screen.height = app->frame.size.height * ratio;
                        
            _renderer->Render();
        }
        
        std::shared_ptr<Renderer> _renderer;
        
        float test = 0.0;
        
        int state = 0;
        
        float lastTick = 0.0;
        float deltaTime = 0.0;

    };

    /**
     * @brief Construct a new Application object
     *
     */
    Application()
    {
        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
        plog::init(plog::verbose, &consoleAppender);
        
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
        
        Global::GetInstance()->InitializeContext(layer, frame);
        
        auto renderer = std::make_shared<Renderer>();
        
        _pViewDelegate->SetRenderer(renderer);
        _pViewDelegate->frame = frame;
        
        this->_context = Global::GetInstance()->GetVulkanContext();

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
    chaiscript::ChaiScript * chai;
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
