#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <filesystem>
#include <vector>

#define NS_PRIVATE_IMPLEMENTATION
#define UI_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>
#include <UIKit/UIKit.hpp>
#include <MetalKit/MetalKit.hpp>

class MyMTKViewDelegate : public MTK::ViewDelegate
{
public:
    MyMTKViewDelegate(MTL::Device *pDevice)
    {
        device = pDevice;
    }
    virtual ~MyMTKViewDelegate() override;
    virtual void drawInMTKView(MTK::View *pView) override;
    MTL::Device *device;
};

MyMTKViewDelegate::~MyMTKViewDelegate()
{
}

void MyMTKViewDelegate::drawInMTKView(MTK::View *pView)
{
    //_pViewDelegate->renderer = new Renderer(ctx);
    pView->draw();
}

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

MyAppDelegate::~MyAppDelegate()
{
    _pMtkView->release();
    _pWindow->release();
    _pDevice->release();
    delete _pViewDelegate;
}

void MyAppDelegate::applicationWillTerminate(UI::Application *pApp)
{
}

bool MyAppDelegate::applicationDidFinishLaunching(UI::Application *pApp, NS::Value *options)
{
    CGRect frame = UI::Screen::mainScreen()->bounds();

    _pWindow = UI::Window::alloc()->init(frame);

    _pViewController = UI::ViewController::alloc()->init(nil, nil);

    _pDevice = MTL::CreateSystemDefaultDevice();

    std::cout << _pDevice->name()->utf8String() << std::endl;

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

    return true;
}

int main(int, char **)
{

    NS::AutoreleasePool *pAutoreleasePool = NS::AutoreleasePool::alloc()->init();

    MyAppDelegate del;
    UI::ApplicationMain(0, 0, &del);

    pAutoreleasePool->release();

    return 0;
}
