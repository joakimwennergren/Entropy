#ifdef BUILD_FOR_IOS

#include <UIKit/UIKit.hpp>

class MTKViewDelegate : public MTK::ViewDelegate
{
public:
    MTKViewDelegate(MTL::Device *pDevice);
    virtual ~MTKViewDelegate() override;
    virtual void drawInMTKView(MTK::View *pView) override;
    Symbios::Graphics::Renderers::Renderer *_renderer;

private:
};

class AppDelegate : public UI::ApplicationDelegate
{
public:
    ~AppDelegate();

    bool applicationDidFinishLaunching(UI::Application *pApp, NS::Value *options) override;
    void applicationWillTerminate(UI::Application *pApp) override;

private:
    UI::Window *_pWindow;
    UI::ViewController *_pViewController;
    MTK::View *_pMtkView;
    MTL::Device *_pDevice;
    MTKViewDelegate *_pViewDelegate = nullptr;
};
#endif