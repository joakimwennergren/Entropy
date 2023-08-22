#ifdef BUILD_FOR_IOS

#define NS_PRIVATE_IMPLEMENTATION
#define UI_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>
#include <UIKit/UIKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include "appdelegate.hpp"


MTKViewDelegate::MTKViewDelegate(MTL::Device *pDevice) : MTK::ViewDelegate()
{
}

MTKViewDelegate::~MTKViewDelegate()
{
    // delete _pRenderer;
}
void MTKViewDelegate::drawInMTKView(MTK::View *pView)
{
    _renderer->Render();
}

bool AppDelegate::applicationDidFinishLaunching(UI::Application *pApp, NS::Value *options)
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

void AppDelegate::applicationWillTerminate(UI::Application *pApp)
{
}

AppDelegate::~AppDelegate()
{
    _pMtkView->release();
    _pWindow->release();
    _pViewController->release();
    _pDevice->release();
    delete _pViewDelegate;
}

#endif

#include "application.hpp"

/**
 * @brief Construct a new Application:: Application object
 *
 */
Application::Application()
{
    // Initialize logger
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)
    // Initialize GLFW
    if (!glfwInit())
    {
        PLOG_FATAL << "Could not initialize GLFW library!";
        return;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Create a windowed mode window
    _window = glfwCreateWindow(640, 480, "Symbios dev application", NULL, NULL);

    if (!_window)
    {
        PLOG_FATAL << "Could not create window!";
        glfwTerminate();
        return;
    }
    
    this->_autoreleasePool = NS::AutoreleasePool::alloc()->init();

    _context = new Symbios::Core::Context(_window);
    _renderer = new Symbios::Graphics::Renderers::Renderer(_context);
    
#endif
}

/**
 * @brief Destroy the Application:: Application object
 *
 */
Application::~Application()
{
#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)
    glfwDestroyWindow(_window);
    glfwTerminate();
#endif
}

/**
 * @brief
 *
 * @return int
 */
void Application::Run()
{
#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)
    while (!glfwWindowShouldClose(_window))
    {
        _renderer->Render();

        glfwPollEvents();
    }
#endif
}
