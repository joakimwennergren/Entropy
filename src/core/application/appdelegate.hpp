#pragma once

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