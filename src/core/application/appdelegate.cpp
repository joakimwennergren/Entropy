#ifdef BUILD_FOR_IOS

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