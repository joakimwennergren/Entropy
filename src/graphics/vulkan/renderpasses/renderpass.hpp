#pragma once

#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/devices/logical_device.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>
#include <graphics/vulkan/textures/depthbuffer_texture.hpp>
#include <graphics/vulkan/textures/swapchain_texture.hpp>
#include <vulkan/vulkan.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>

using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::SwapChains;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::Memory;

namespace Entropy::Graphics::Vulkan::RenderPasses {
    struct RenderPass {
        /**
         * @brief Constructs a RenderPass object and initializes its various components.
         *
         * This constructor fetches the necessary services from the ServiceLocator,
         * including the logical device, physical device, and swapchain.
         * It verifies that these services are properly fetched and asserts non-null values.
         *
         * It recreates the depth buffer based on the swapchain's dimensions and sets up
         * the Vulkan attachment descriptions, reference attachments, subpass dependencies,
         * and the subpass itself for both color and depth attachments.
         *
         * Finally, it creates the Vulkan render pass using the assembled configurations.
         *
         * @throws std::runtime_error if the Vulkan render pass creation fails.
         */
        RenderPass() {
            const ServiceLocator *sl = ServiceLocator::GetInstance();
            _logicalDevice = sl->getService<ILogicalDevice>();
            _physicalDevice = sl->getService<IPhysicalDevice>();
            _swapChain = sl->getService<ISwapchain>();

            assert(_logicalDevice != nullptr);
            assert(_physicalDevice != nullptr);

            RecreateDepthBuffer(_swapChain->swapChainExtent.width,
                                _swapChain->swapChainExtent.height);

            VkAttachmentDescription depthAttachment{};
            depthAttachment.format = FindDepthFormat();
            depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            depthAttachment.finalLayout =
                    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentReference depthAttachmentRef{};
            depthAttachmentRef.attachment = 1;
            depthAttachmentRef.layout =
                    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentDescription colorAttachment{};
            colorAttachment.format = VK_FORMAT_R8G8B8A8_UNORM; // VK_FORMAT_R8G8B8A8_UNORM; // VK_FORMAT_B8G8R8A8_UNORM
            colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkSubpassDependency dependency{};
            dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                                      VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                                      VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                                       VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

            VkAttachmentReference colorAttachmentRef{};
            colorAttachmentRef.attachment = 0;
            colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass{};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &colorAttachmentRef;
            subpass.pDepthStencilAttachment = &depthAttachmentRef;

            std::array<VkAttachmentDescription, 2> attachments = {
                colorAttachment,
                depthAttachment
            };
            VkRenderPassCreateInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            renderPassInfo.pAttachments = attachments.data();
            renderPassInfo.subpassCount = 1;
            renderPassInfo.pSubpasses = &subpass;
            renderPassInfo.dependencyCount = 1;
            renderPassInfo.pDependencies = &dependency;

            VK_CHECK(vkCreateRenderPass(_logicalDevice->Get(), &renderPassInfo, nullptr,
                &_renderPass));
        }

        /**
         * @brief Begins a Vulkan render pass with the specified parameters.
         *
         * This method sets up the render pass begin information structure,
         * initializes the render area, specifies the framebuffer for the given image index,
         * and defines clear values for the color and depth attachments.
         * It then begins the render pass on the provided command buffer.
         *
         * @param commandBuffer The command buffer on which to begin the render pass.
         * @param imageIndex The index of the current swapchain image.
         * @param width The width of the render area.
         * @param height The height of the render area.
         */
        void Begin(CommandBuffer commandBuffer, uint32_t imageIndex, int width,
                   int height) const;

        /**
         * @brief Ends the currently active Vulkan render pass associated with the given command buffer.
         *
         * This method issues a command to end the render pass that was previously started.
         * It is essential to pair this call associated with a specific command buffer.
         * The operation finalizes recording of rendering commands within the render pass,
         * making it ready for execution during command buffer submission.
         *
         * @param commandBuffer The command buffer in which the render pass is active.
         */
        static void End(CommandBuffer commandBuffer);

        /**
         * @brief Recreates the framebuffers with the specified dimensions and application state.
         *
         * This method destroys the existing framebuffers and creates new ones with either
         * the provided width and height or based on the swapchain dimensions depending on
         * the provided application state.
         *
         * @param width The width of the new framebuffers.
         * @param height The height of the new framebuffers.
         * @param app A boolean flag indicating whether the framebuffers should be created
         *        using the swapchain (true) or directly with the provided dimensions (false).
         */
        void RecreateFrameBuffers(const int width, const int height) {
            for (const auto framebuffer: _frameBuffers) {
                vkDestroyFramebuffer(_logicalDevice->Get(), framebuffer, nullptr);
            }
            CreateFrameBuffers(_swapChain, width, height);
        }

        /**
         * @brief Recreates the depth buffer with the specified dimensions.
         *
         * This method initializes a new DepthBufferTexture object with the provided width and height.
         * The depth buffer is necessary for depth testing during the rendering process.
         *
         * @param width The width of the depth buffer.
         * @param height The height of the depth buffer.
         */
        void RecreateDepthBuffer(uint32_t width, uint32_t height) {
            _depthBufferTexture = std::make_shared<DepthBufferTexture>(width, height);
        }

        [[nodiscard]] VkRenderPass Get() const { return _renderPass; }

        /**
         * @brief Creates framebuffers for the current swap chain and depth buffer textures.
         *
         * This method initializes and configures the framebuffers necessary for rendering. It clears
         * any existing swap chain textures and framebuffers, then creates a new swap chain texture
         * with the specified width and height. It configures framebuffer attachments for both the
         * swap chain texture and depth buffer texture, sets up the framebuffer creation information,
         * and finally creates the framebuffer using the Vulkan API.
         *
         * @param width The width of the framebuffer and swap chain texture.
         * @param height The height of the framebuffer and swap chain texture.
         *
         * @throw std::runtime_error If the Vulkan framebuffer creation fails.
         */
        void CreateFrameBuffers(int width, int height) {
            _swapChainTextures.clear();
            _frameBuffers.clear();
            _swapChainTextures.push_back(
                std::make_shared<SwapChainTexture>(width, height));
            _frameBuffers.resize(1);

            const std::array<VkImageView, 2> attachments = {
                _swapChainTextures[0]->imageView->Get(),
                _depthBufferTexture->imageView->Get()
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = _renderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = width;
            framebufferInfo.height = height;
            framebufferInfo.layers = 1;

            VK_CHECK(vkCreateFramebuffer(_logicalDevice->Get(), &framebufferInfo,
                nullptr, &_frameBuffers[0]));
        }

        void CreateFrameBuffers(const std::shared_ptr<ISwapchain> &swapChain, const int width,
                                const int height) {
            _swapChainTextures.clear();
            _frameBuffers.clear();
            _frameBuffers.resize(3);

            for (size_t i = 0; i < swapChain->_swapChainImageViews.size(); i++) {
                std::array<VkImageView, 2> attachments = {
                    swapChain->_swapChainImageViews[i]->Get(),
                    _depthBufferTexture->imageView->Get()
                };

                VkFramebufferCreateInfo framebufferInfo{};
                framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                framebufferInfo.renderPass = _renderPass;
                framebufferInfo.attachmentCount =
                        static_cast<uint32_t>(attachments.size());
                framebufferInfo.pAttachments = attachments.data();
                framebufferInfo.width = width;
                framebufferInfo.height = height;
                framebufferInfo.layers = 1;

                VK_CHECK(vkCreateFramebuffer(
                    _logicalDevice->Get(), &framebufferInfo, nullptr, &_frameBuffers[i]));
            }
        }

        std::vector<VkFramebuffer> _frameBuffers;
        std::vector<std::shared_ptr<SwapChainTexture> > _swapChainTextures;

    private:
        std::shared_ptr<DepthBufferTexture> _depthBufferTexture = nullptr;
        VkRenderPass _renderPass = VK_NULL_HANDLE;

        [[nodiscard]] VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                                                   VkImageTiling tiling,
                                                   VkFormatFeatureFlags features) const;

        [[nodiscard]] VkFormat FindDepthFormat() const;

        std::shared_ptr<ILogicalDevice> _logicalDevice;
        std::shared_ptr<IPhysicalDevice> _physicalDevice;
        std::shared_ptr<ISwapchain> _swapChain;
    };
} // namespace Entropy::Graphics::Vulkan::RenderPasses
