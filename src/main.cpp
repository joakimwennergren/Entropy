#include "config.h"

#if BUILD_FOR_IOS == true

#include <iostream>
#include <vulkan/vulkan.hpp>

#define NS_PRIVATE_IMPLEMENTATION
#define UI_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>
#include <UIKit/UIKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include "context.hpp"

class MyMTKViewDelegate : public MTK::ViewDelegate
{
public:
    MyMTKViewDelegate(MTL::Device *pDevice);
    virtual ~MyMTKViewDelegate() override;
    virtual void drawInMTKView(MTK::View *pView) override;

private:
    // Renderer *_pRenderer;
};

class MyAppDelegate : public UI::ApplicationDelegate
{
public:
    ~MyAppDelegate();

    bool applicationDidFinishLaunching(UI::Application *pApp, NS::Value *options) override;
    void applicationWillTerminate(UI::Application *pApp) override;

private:
    UI::Window *_pWindow;
    UI::ViewController *_pViewController;
    MTK::View *_pMtkView;
    MTL::Device *_pDevice;
    MyMTKViewDelegate *_pViewDelegate = nullptr;
};

int main(int argc, const char **argv)
{
    NS::AutoreleasePool *_pAutoreleasePool = NS::AutoreleasePool::alloc()->init();

    MyAppDelegate del;
    UI::ApplicationMain(0, 0, &del);

    _pAutoreleasePool->release();
}

MyMTKViewDelegate::MyMTKViewDelegate(MTL::Device *pDevice)
    : MTK::ViewDelegate()
{
}

MyMTKViewDelegate::~MyMTKViewDelegate()
{
    // delete _pRenderer;
}

void MyMTKViewDelegate::drawInMTKView(MTK::View *pView)
{
    //_pRenderer->draw(pView);
    // std::cout << "REndering!!!" << std::endl;
}

MyAppDelegate::~MyAppDelegate()
{
    _pMtkView->release();
    _pWindow->release();
    _pViewController->release();
    _pDevice->release();
    delete _pViewDelegate;
}

bool MyAppDelegate::applicationDidFinishLaunching(UI::Application *pApp, NS::Value *options)
{
    CGRect frame = UI::Screen::mainScreen()->bounds();

    _pWindow = UI::Window::alloc()->init(frame);

    _pViewController = UI::ViewController::alloc()->init(nil, nil);

    _pDevice = MTL::CreateSystemDefaultDevice();

    _pMtkView = MTK::View::alloc()->init(frame, _pDevice);
    _pMtkView->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
    _pMtkView->setClearColor(MTL::ClearColor::Make(1.0, 0.0, 0.0, 1.0));

    _pViewDelegate = new MyMTKViewDelegate(_pDevice);
    _pMtkView->setDelegate(_pViewDelegate);

    UI::View *mtkView = (UI::View *)_pMtkView;
    mtkView->setAutoresizingMask(UI::ViewAutoresizingFlexibleWidth | UI::ViewAutoresizingFlexibleHeight);
    _pViewController->view()->addSubview(mtkView);
    _pWindow->setRootViewController(_pViewController);

    _pWindow->makeKeyAndVisible();

    CA::MetalLayer *layer;
    layer = _pMtkView->currentDrawable()->layer();

    auto context = Symbios::Core::Context(layer);

    return true;
}

void MyAppDelegate::applicationWillTerminate(UI::Application *pApp)
{
}
#endif

#if BUILD_FOR_MACOS == true || BUILD_FOR_WINDOWS == true || BUILD_FOR_LINUX == true

#include "application.hpp"

int main()
{
    Application app;
    app.Run();
    return 0;
}

#endif
