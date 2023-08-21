#define NS_PRIVATE_IMPLEMENTATION
#define UI_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>
#include <UIKit/UIKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include "application.hpp"

#ifdef BUILD_FOR_IOS

class MTKViewDelegate : public MTK::ViewDelegate
{
public:
    MTKViewDelegate(MTL::Device *pDevice);
    virtual ~MTKViewDelegate() override;
    virtual void drawInMTKView(MTK::View *pView);

    Symbios::Graphics::CommandBuffers::CommandBuffer *commandBuffer;
    Symbios::Graphics::RenderPasses::Default *renderPass;
    Symbios::Graphics::Pipeline::Default *pipeline;
    Symbios::Core::Context *_context;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

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

MTKViewDelegate::MTKViewDelegate(MTL::Device *pDevice) : MTK::ViewDelegate()
{
}

MTKViewDelegate::~MTKViewDelegate()
{
    // delete _pRenderer;
}
void MTKViewDelegate::drawInMTKView(MTK::View *pView)
{
    pView->currentDrawable()->present();

    vkWaitForFences(_context->GetLogicalDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(_context->GetLogicalDevice(), _context->GetSwapChain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    vkResetCommandBuffer(commandBuffer->GetCommandBuffer(), 0);

    commandBuffer->Record(0, renderPass);

    vkCmdBindPipeline(commandBuffer->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(_context->GetSwapChainExtent().width);
    viewport.height = static_cast<float>(_context->GetSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer->GetCommandBuffer(), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = _context->GetSwapChainExtent();
    vkCmdSetScissor(commandBuffer->GetCommandBuffer(), 0, 1, &scissor);

    vkCmdDraw(commandBuffer->GetCommandBuffer(), 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer->GetCommandBuffer());

    if (vkEndCommandBuffer(commandBuffer->GetCommandBuffer()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }

    auto cmd = commandBuffer->GetCommandBuffer();

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmd;

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(_context->_graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {_context->GetSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResults = nullptr; // Optional

    vkQueuePresentKHR(_context->_presentQueue, &presentInfo);

    vkResetFences(_context->GetLogicalDevice(), 1, &inFlightFence);
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

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    auto context = new Symbios::Core::Context(layer, frame);

    auto renderPass = new Symbios::Graphics::RenderPasses::Default(context);

    auto pipeline = new Symbios::Graphics::Pipeline::Default(context, renderPass);

    auto commandBuffer = new Symbios::Graphics::CommandBuffers::CommandBuffer(context);

    _pViewDelegate->_context = context;
    _pViewDelegate->renderPass = renderPass;
    _pViewDelegate->pipeline = pipeline;
    _pViewDelegate->commandBuffer = commandBuffer;

    if (vkCreateSemaphore(context->GetLogicalDevice(), &semaphoreInfo, nullptr, &_pViewDelegate->imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(context->GetLogicalDevice(), &semaphoreInfo, nullptr, &_pViewDelegate->renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(context->GetLogicalDevice(), &fenceInfo, nullptr, &_pViewDelegate->inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create semaphores!");
    }

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

/**
 * @brief Construct a new Application:: Application object
 *
 */
Application::Application()
{
    // Initialize logger
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS)
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

    /*

    // Create Vulkan context
    _context = new Symbios::Core::Context(_window);

    _renderPass = new Symbios::Graphics::RenderPasses::Default(_context);

    _pipeline = new Symbios::Graphics::Pipeline::Default(_context, _renderPass);

    _commandBuffer = new Symbios::Graphics::CommandBuffers::CommandBuffer(_context);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateSemaphore(_context->GetLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(_context->GetLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(_context->GetLogicalDevice(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create semaphores!");
    }
    */

    this->_autoreleasePool = NS::AutoreleasePool::alloc()->init();

    // this->_autoreleasePool->release();

#endif
}

/**
 * @brief Destroy the Application:: Application object
 *
 */
Application::~Application()
{
#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS)
    glfwDestroyWindow(_window);
    glfwTerminate();
#endif

#ifdef BUILD_FOR_IOS

#endif
}

/**
 * @brief
 *
 * @return int
 */
void Application::Run()
{
#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS)

    while (!glfwWindowShouldClose(_window))
    {
        /*
        vkWaitForFences(_context->GetLogicalDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(_context->GetLogicalDevice(), _context->GetSwapChain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

        vkResetCommandBuffer(_commandBuffer->GetCommandBuffer(), 0);

        _commandBuffer->Record(0, _renderPass);

        vkCmdBindPipeline(_commandBuffer->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->pipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(_context->GetSwapChainExtent().width);
        viewport.height = static_cast<float>(_context->GetSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(_commandBuffer->GetCommandBuffer(), 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = _context->GetSwapChainExtent();
        vkCmdSetScissor(_commandBuffer->GetCommandBuffer(), 0, 1, &scissor);

        vkCmdDraw(_commandBuffer->GetCommandBuffer(), 3, 1, 0, 0);

        vkCmdEndRenderPass(_commandBuffer->GetCommandBuffer());

        _commandBuffer->EndRecording();

        auto cmd = _commandBuffer->GetCommandBuffer();

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cmd;

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(_context->_graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {_context->GetSwapChain()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        presentInfo.pResults = nullptr; // Optional

        vkQueuePresentKHR(_context->_presentQueue, &presentInfo);

        //vkResetFences(_context->GetLogicalDevice(), 1, &inFlightFence);
        */

        glfwPollEvents();
    }
#endif

#ifdef BUILD_FOR_IOS
    AppDelegate del;
    UI::ApplicationMain(0, 0, &del);
#endif
}
