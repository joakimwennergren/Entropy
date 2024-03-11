/**
    application.hpp
    Application wrapper interface, common for all platforms
    @author Joakim Wennergren
    @version 1.0 2/11/2023
*/
#pragma once

#include <thread>
#include <memory>
#include <config.hpp>

#include <flecs/flecs.h>

#include <graphics/renderers/renderer.hpp>
#include <servicelocators/servicelocator.hpp>
#include <timing/timer.hpp>
#include <graphics/devices/logical_device.hpp>

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
#include <graphics/commandbuffers/commandbuffer.hpp>
#include <physics/2d/physics2d.hpp>
#include <scripting/lua.hpp>
#include <input/mouse/mouse.hpp>
#include <filesystem/filesystem.hpp>
#include <input/keyboard/keyboard.hpp>
#include <graphics/cameras/flying_camera.hpp>
#include <renderables/renderable.hpp>
#include <graphics/utilities/utilities.hpp>
#include <graphics/textures/texture.hpp>
#include <graphics/buffers/buffer.hpp>
#include <graphics/buffers/vertexbuffer.hpp>
#include <graphics/memory/allocator.hpp>
#include <physics/3d/physics3d.hpp>
#include <ecs/world.hpp>
#include <graphics/synchronization/queuesync.hpp>

#include <graphics/primitives/2d/line.hpp>

using namespace Entropy::Graphics::Instances;
using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::Swapchains;
using namespace Entropy::Graphics::ImageViews;
using namespace Entropy::Graphics::CommandPools;
using namespace Entropy::Graphics::CommandBuffers;
using namespace Entropy::Graphics::DescriptorPools;
using namespace Entropy::Graphics::DescriptorsetLayouts;
using namespace Entropy::Graphics::Descriptorsets;
using namespace Entropy::Scripting;
using namespace Entropy::Physics;
using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Textures;
using namespace Entropy::Graphics::Renderers;
using namespace Entropy::Renderables;
using namespace Entropy;
using namespace Entropy::Graphics::Synchronization;
using namespace Entropy::Input;
using namespace Entropy::Graphics::Buffers;
using namespace Entropy::Graphics::Utilities;
using namespace Entropy::Graphics::Memory;
using namespace Entropy::ECS;

using namespace std::chrono_literals;

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)

#include <GLFW/glfw3.h>

using namespace Entropy::Graphics::Renderers;
using namespace Entropy::Timing;
using namespace Entropy::ServiceLocators;

void framebufferResizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void window_refresh_callback(GLFWwindow *window);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void character_callback(GLFWwindow *window, unsigned int codepoint);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void window_iconify_callback(GLFWwindow *window, int iconified);

class Application
{
public:
    Application();
    ~Application();

    virtual void OnInit() = 0;

    virtual void OnRender(float deltaTime) = 0;

    void Run();

    // @todo this shouldn't be public
    inline std::shared_ptr<Renderer> GetRenderer() { return this->_renderer; };
    std::shared_ptr<Keyboard> _keyboard;
    std::shared_ptr<Cam> _camera;
    bool firstMouse;
    float lastX = 0.0;
    float lastY = 0.0;
    std::shared_ptr<ServiceLocator> serviceLocator;
    // @todo look over if this should be protected..
    std::shared_ptr<Renderer> _renderer;
    std::shared_ptr<Mouse> mouse;
    bool isResizing = false;
    std::unique_ptr<Entropy::Timing::Timer> _timer;
    float mouse_x;
    int nbFrames = 0;
    float lastTime;
    float mouse_y;
    float screen_width;
    float screen_height;
    bool mouse0_state = false;
    bool isMinimized = false;

protected:
    GLFWwindow *_window;
    std::vector<std::shared_ptr<CommandBuffer>> commandBuffers;
    std::shared_ptr<VulkanInstance> _vkInstance;
    std::shared_ptr<WindowSurface> _windowSurface;
    std::shared_ptr<WindowSurface> _windowSurface2;
    std::shared_ptr<PhysicalDevice> _physicalDevice;
    std::shared_ptr<LogicalDevice> _logicalDevice;
    std::shared_ptr<Allocator> _allocator;
    std::shared_ptr<DescriptorPool> _descriptorPool;
    std::shared_ptr<Swapchain> _swapChain;
    std::shared_ptr<Lua> lua;
    std::shared_ptr<Physics2D> physics2d;
    std::shared_ptr<RenderPass> renderpass;
    std::shared_ptr<Synchronizer> synchronizer;
    std::shared_ptr<Physics3D> physics3d;
    std::shared_ptr<World> world;
    sol::protected_function luaOnRender;

private:
    float _lastTick = 0.0f;
    float _deltaTime = 0.0f;
};
#endif

#ifdef BUILD_FOR_IOS

#include <Metal/Metal.hpp>
#include <UIKit/UIKit.hpp>
#include <MetalKit/MetalKit.hpp>
#include <MetalFX/MetalFX.hpp>

using namespace std::chrono;

extern "C" UI::ViewController *get_native_bounds(UI::View *view, UI::Screen *screen);
extern "C" CGPoint touch();
extern "C" void say_hello();

class EntropyApplication : public UI::ApplicationDelegate
{
public:
    std::shared_ptr<Keyboard> _keyboard;
    std::shared_ptr<Cam> _camera;
    bool firstMouse;
    float lastX = 0.0;
    float lastY = 0.0;

    std::shared_ptr<ServiceLocator> serviceLocator;
    std::shared_ptr<Physics2D> physics2d;
    sol::protected_function luaOnRender;
    std::shared_ptr<Renderer> _renderer;

    class MTKViewDelegate : public MTK::ViewDelegate
    {
    public:
        MTKViewDelegate(EntropyApplication *app) : MTK::ViewDelegate()
        {
            this->app = app;
        }

        virtual ~MTKViewDelegate() override
        {
        }
        EntropyApplication *app;
        std::shared_ptr<Renderer> _renderer;
        CGRect frame;

    private:
        /**
         * @brief
         *
         * @param pView
         */
        inline virtual void drawInMTKView(MTK::View *pView) override
        {
            /*
            auto tick_time = (float)GetTimeAsDouble();

            deltaTime = tick_time - lastTick;

            lastTick = tick_time;

            app->OnRender(deltaTime);

            auto ratio = (app->frame.size.width) / (app->frame.size.height);

            app->screen.width = app->frame.size.width * 3.0;
            app->screen.height = app->frame.size.height * 3.3;
             */

            _renderer->Render(frame.size.width, frame.size.height);
            app->OnRender(0.0);
        }

        float lastTick = 0.0;
        float deltaTime = 0.0;
    };

    /**
     * @brief Construct a new Application object
     *
     */
    EntropyApplication()
    {
        say_hello();
        this->_autoreleasePool = NS::AutoreleasePool::alloc()->init();
    }

    /**
     * @brief Destroy the Application object
     *
     */
    ~EntropyApplication()
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
        frame = UI::Screen::mainScreen()->bounds();

        VkExtent2D extent =
            {
                (uint32_t)frame.size.width,
                (uint32_t)frame.size.height};

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

        // Create items for vulkan
        _vkInstance = std::make_shared<VulkanInstance>("Entropy tests");
        auto windowSurface = std::make_shared<WindowSurface>(vkInstance, layer);
        auto physicalDevice = std::make_shared<PhysicalDevice>(vkInstance, windowSurface);
        auto logicalDevice = std::make_shared<LogicalDevice>(physicalDevice, windowSurface);
        auto swapChain = std::make_shared<Swapchain>(physicalDevice->Get(), logicalDevice->Get(), windowSurface, extent);
        auto commandPool = std::make_shared<CommandPool>(logicalDevice, physicalDevice, windowSurface);
        auto descriptorPool = std::make_shared<DescriptorPool>(logicalDevice);

        // Add services to service locator
        serviceLocator = std::make_shared<ServiceLocator>();

        _keyboard = std::make_shared<Keyboard>(serviceLocator);
        _camera = std::make_shared<Cam>(glm::vec3(0.0f, 0.0f, 3.0f));
        serviceLocator->AddService(_camera);
        serviceLocator->AddService(_keyboard);
        serviceLocator->AddService(physicalDevice);
        serviceLocator->AddService(logicalDevice);
        serviceLocator->AddService(descriptorPool);
        serviceLocator->AddService(swapChain);
        serviceLocator->AddService(commandPool);
        physics2d = std::make_shared<Physics2D>(serviceLocator);
        serviceLocator->AddService(physics2d);

        _renderer = std::make_shared<Renderer>(serviceLocator);
        _pViewDelegate->_renderer = _renderer;
        _pViewDelegate->frame = frame;
        _pViewDelegate->app = this;

        OnInit();

        return true;
    }

    /**
     * @brief
     *
     * @param pApp
     * @param options
     * @return true
     * @return false
     */
    /*
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


        OnInit();

        return true;
    }

*/
    virtual void OnInit() = 0;

    virtual void OnRender(float deltaTime) = 0;

    /**
     * @brief
     *
     * @param pApp
     */
    /*
    inline void applicationWillTerminate(UI::Application *pApp) override
    {
    }
    */

public:
    /**
     * @brief
     *
     */
    inline void Run()
    {
        UI::ApplicationMain(0, 0, this);
    }

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
