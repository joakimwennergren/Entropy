#pragma once

#include "graphics/renderers/vulkan_renderer.hpp"
#include <config.hpp>
#include <memory>
#include <thread>

#include <flecs/flecs.h>
#include <kangaru/kangaru.hpp>

#include <graphics/vulkan/devices/logical_device.hpp>
#include <timing/timer.hpp>

#include <ecs/components/renderable.hpp>

#include <services/entity_factory_service.hpp>
#include <services/vulkan_render_service.hpp>
#include <services/world_service.hpp>

// new includes
#include <ecs/world.hpp>
#include <filesystem/filesystem.hpp>
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/commandpools/commandpool.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/descriptorsetlayouts/descriptorsetlayout.hpp>
#include <graphics/vulkan/descriptorsets/descriptorset.hpp>
#include <graphics/vulkan/devices/physical_device.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/instances/vk_instance.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/surfaces/surface.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>
#include <graphics/vulkan/synchronization/queuesync.hpp>
#include <graphics/vulkan/textures/texture.hpp>
#include <input/keyboard/keyboard.hpp>
#include <input/mouse/mouse.hpp>
#include <physics/2d/physics2d.hpp>
#include <physics/3d/physics3d.hpp>
#include <scripting/lua.hpp>

#include <graphics/primitives/2d/line.hpp>

using namespace Entropy::Graphics::Vulkan::Instances;
using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::Swapchains;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::CommandPools;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Graphics::Vulkan::DescriptorsetLayouts;
using namespace Entropy::Graphics::Vulkan::Descriptorsets;
using namespace Entropy::Scripting;
using namespace Entropy::Physics;
using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Input;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::ECS;
using namespace Entropy::Graphics::Services;

using namespace std::chrono_literals;

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) ||                  \
    defined(BUILD_FOR_LINUX)

#include <GLFW/glfw3.h>

using namespace Entropy::Timing;

void framebufferResizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void window_refresh_callback(GLFWwindow *window);
void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods);
void character_callback(GLFWwindow *window, unsigned int codepoint);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void window_iconify_callback(GLFWwindow *window, int iconified);
void window_content_scale_callback(GLFWwindow *window, float xscale,
                                   float yscale);

namespace Entropy {
class Application {
public:
  Application();
  ~Application();

  virtual void OnInit() = 0;
  virtual void OnRender(float deltaTime) = 0;

  void Run();

  // @todo this shouldn't be public
  std::shared_ptr<Keyboard> _keyboard;
  bool firstMouse;
  float lastX = 0.0;
  float lastY = 0.0;
  // @todo look over if this should be protected..
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
  float xscale = 1.0;
  float yscale = 1.0;

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
  std::shared_ptr<Physics3D> physics3d;
  std::shared_ptr<World> world;
  sol::protected_function luaOnRender;

private:
  float _lastTick = 0.0f;
  float _deltaTime = 0.0f;
};
} // namespace Entropy

#endif

#ifdef BUILD_FOR_IOS

// #include <Metal/Metal.hpp>
// #include <MetalFX/MetalFX.hpp>
// #include <MetalKit/MetalKit.hpp>
// #include <UIKit/UIKit.hpp>

// extern "C" UI::ViewController *get_native_bounds(UI::View *view,
//                                                  UI::Screen *screen);
// extern "C" CGPoint touch();
// extern "C" void say_hello();

// class EntropyApplication : public UI::ApplicationDelegate {
// public:
//   class MTKViewDelegate : public MTK::ViewDelegate {
//   public:
//     MTKViewDelegate(EntropyApplication *app) : MTK::ViewDelegate() {
//       this->app = app;
//     }

//     virtual ~MTKViewDelegate() override {}
//     EntropyApplication *app;
//     Renderers::VulkanRenderer *renderer;
//     CGRect frame;

//   private:
//     /**
//      * @brief
//      *
//      * @param pView
//      */
//     inline virtual void drawInMTKView(MTK::View *pView) override {
//       /*
//       auto tick_time = (float)GetTimeAsDouble();

//       deltaTime = tick_time - lastTick;

//       lastTick = tick_time;

//       app->OnRender(deltaTime);

//       auto ratio = (app->frame.size.width) / (app->frame.size.height);

//       app->screen.width = app->frame.size.width * 3.0;
//       app->screen.height = app->frame.size.height * 3.3;
//        */
//       bool shouldResize = false;
//       renderer->Render(frame.size.width, frame.size.height, 1.0, 1.0,
//                        shouldResize);
//     }

//     float lastTick = 0.0;
//     float deltaTime = 0.0;
//   };

//   /**
//    * @brief Construct a new Application object
//    *
//    */
//   EntropyApplication() {
//     this->_autoreleasePool = NS::AutoreleasePool::alloc()->init();
//   }

//   /**
//    * @brief Destroy the Application object
//    *
//    */
//   ~EntropyApplication() {
//     _pMtkView->release();
//     _pWindow->release();
//     _pViewController->release();
//     _pDevice->release();
//     delete _pViewDelegate;
//     this->_autoreleasePool->release();
//   }

//   inline bool applicationDidFinishLaunching(UI::Application *pApp,
//                                             NS::Value *options) override {

//     spdlog::error("Do we even get here?");
//     frame = UI::Screen::mainScreen()->bounds();

//     VkExtent2D extent = {(uint32_t)frame.size.width,
//                          (uint32_t)frame.size.height};

//     //_pViewController = UI::ViewController::alloc()->init(nil, nil);

//     _pWindow = UI::Window::alloc()->init(frame);

//     _pDevice = MTL::CreateSystemDefaultDevice();

//     _pMtkView = MTK::View::alloc()->init(frame, _pDevice);

//     _pViewController =
//         get_native_bounds((UI::View *)_pMtkView, UI::Screen::mainScreen());

//     _pMtkView->setColorPixelFormat(
//         MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
//     _pMtkView->setClearColor(MTL::ClearColor::Make(1.0, 1.0, 1.0, 1.0));

//     _pViewDelegate = new MTKViewDelegate(this);
//     _pMtkView->setDelegate(_pViewDelegate);

//     UI::View *mtkView = (UI::View *)_pMtkView;
//     mtkView->setAutoresizingMask(UI::ViewAutoresizingFlexibleWidth |
//                                  UI::ViewAutoresizingFlexibleHeight);

//     _pViewController->view()->addSubview(mtkView);
//     _pWindow->setRootViewController(_pViewController);

//     _pWindow->makeKeyAndVisible();

//     CA::MetalLayer *layer = _pMtkView->currentDrawable()->layer();

//     kgr::container vulkanContainer;

//     auto renderer = vulkanContainer.service<VulkanRenderService>();
//     auto entityFactory = vulkanContainer.service<EntityFactoryService>();

//     _pViewDelegate->renderer = &renderer;
//     _pViewDelegate->frame = frame;
//     _pViewDelegate->app = this;

//     OnInit();

//     return true;
//   }

//   /**
//    * @brief
//    *
//    * @param pApp
//    * @param options
//    * @return true
//    * @return false
//    */
//   /*
//   inline bool applicationDidFinishLaunching(UI::Application *pApp, NS::Value
//   *options) override
//   {
//       frame = UI::Screen::mainScreen()->bounds();

//       //_pViewController = UI::ViewController::alloc()->init(nil, nil);

//       _pWindow = UI::Window::alloc()->init(frame);

//       _pDevice = MTL::CreateSystemDefaultDevice();

//       _pMtkView = MTK::View::alloc()->init(frame, _pDevice);

//       _pViewController = get_native_bounds((UI::View *)_pMtkView,
//   UI::Screen::mainScreen());

//       _pMtkView->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
//       _pMtkView->setClearColor(MTL::ClearColor::Make(1.0, 1.0, 1.0, 1.0));

//       _pViewDelegate = new MTKViewDelegate(this);
//       _pMtkView->setDelegate(_pViewDelegate);

//       UI::View *mtkView = (UI::View *)_pMtkView;
//       mtkView->setAutoresizingMask(UI::ViewAutoresizingFlexibleWidth |
//   UI::ViewAutoresizingFlexibleHeight);

//       _pViewController->view()->addSubview(mtkView);
//       _pWindow->setRootViewController(_pViewController);

//       _pWindow->makeKeyAndVisible();

//       OnInit();

//       return true;
//   }

// */
//   virtual void OnInit() = 0;

//   virtual void OnRender(float deltaTime) = 0;

//   /**
//    * @brief
//    *
//    * @param pApp
//    */
//   /*
//   inline void applicationWillTerminate(UI::Application *pApp) override
//   {
//   }
//   */

// public:
//   /**
//    * @brief
//    *
//    */
//   inline void Run() { UI::ApplicationMain(0, 0, this); }

//   CGRect frame;

// protected:
// private:
//   UI::Window *_pWindow = nullptr;
//   MTK::View *_pMtkView = nullptr;
//   MTL::Device *_pDevice = nullptr;
//   MTKViewDelegate *_pViewDelegate = nullptr;
//   NS::AutoreleasePool *_autoreleasePool = nullptr;
//   UI::ViewController *_pViewController = nullptr;
// };

#endif
