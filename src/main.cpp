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
    std::cout << "REndering!!!" << std::endl;
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

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "available extensions:\n";

    for (const auto &extension : extensions)
    {
        std::cout << '\t' << extension.extensionName << '\n';
    }

    /*
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create instance!");
        }
        */

    return true;
}

void MyAppDelegate::applicationWillTerminate(UI::Application *pApp)
{
}
#endif

#if BUILD_FOR_DESKTOP == true

#include "application.hpp"

int main()
{
    Application app;
    app.Run();
    return 0;
}

#endif
