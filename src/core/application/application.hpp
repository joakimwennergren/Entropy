#pragma once

#include "config.hpp"
#include "context.hpp"
#include "renderer.hpp"

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include "plog/Initializers/RollingFileInitializer.h"

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)

#include <GLFW/glfw3.h>

using namespace Symbios;
using namespace Symbios::Core;
using namespace Symbios::Graphics::Renderers;
using namespace Symbios::SceneGraphs;

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
        _window = glfwCreateWindow(640, 480, "Symbios dev application", NULL, NULL);

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
        _sceneGraph = std::make_shared<SceneGraph>();

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
            _renderer->Render(_sceneGraph);
        }
    }

protected:
        std::shared_ptr<Context> _context;
        std::shared_ptr<SceneGraph> _sceneGraph;
private:

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
        
        /*
        float SpawnMushrooms()
        {
            for(int i = 0; i < 5; ++i)
            {
                auto svamp = std::make_shared<Sprite>();
                float x = 100.0;
                float y = 380.0;
                
                if(i < 2)
                {
                    x = RandomFloat(550.0, frame.size.width * 3.0 - 300.0);
                    y = RandomFloat(360.0, 400.0);
                    svamp->zIndex = 9;
                }
                
                if(i >=2 && i <= 4)
                {
                    x = RandomFloat(550.0, frame.size.width * 3.0 - 300.0);
                    y = RandomFloat(600.0, 680.0);
                    svamp->zIndex = 7;
                }
                
                svamp->id = i + 1;
                svamp->rotationX = RandomFloat(-35.0, 35.0);
                svamp->New(
                           Filesystem::GetProjectBasePath() + mushrooms[rand() % mushrooms.size()],
                           glm::vec3(x, y * -1.0f, 0.0),
                           glm::vec3(100.0, 100.0, 0.0),
                           glm::vec4(1.0, 1.0, 1.0, 1.0)
                           );
                graph->renderables.push_back(svamp);
            }
         
        }
         */
        
        
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
                        
            _renderer->Render();
        }
        
        std::shared_ptr<Renderer> _renderer;
        
        float test = 0.0;
        
        int state = 0;
        
        float lastTick = 0.0;
        float deltaTime = 0.0;
        
        // MushroomHunter GameState
        bool hasSpawnedMushrooms = false;
        
        const std::vector<std::string> mushrooms = {
            "/Eldsopp.png",
            "/Bombmurkla.png",
            "/gul_kantarell.png",
            "/Stolt fjällskivling.png",
            "/Vaxskivling.png",
        };
        
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
