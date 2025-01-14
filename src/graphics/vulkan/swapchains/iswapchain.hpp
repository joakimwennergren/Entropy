#ifndef ENTROPY_ISWAPCHAIN_H
#define ENTROPY_ISWAPCHAIN_H

#include <servicelocators/servicelocator.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <vulkan/vulkan.hpp>

namespace Entropy::Graphics::Vulkan::SwapChains {
    struct ISwapchain : IService {
        /**
         * @brief Destructor for the ISwapchain interface.
         *
         * This is the default virtual destructor for the ISwapchain interface.
         * It ensures that derived classes can clean up resources properly
         * when an ISwapchain object is destroyed.
         */
        ~ISwapchain() override = default;

        /**
         * @brief Retrieves the Vulkan swapchain handle.
         *
         * This method provides access to the Vulkan swapchain handle (VkSwapchainKHR),
         * which is used to manage the presentation of rendered images to the screen.
         * The implementation of this method must be provided by derived classes.
         *
         * @return The Vulkan swapchain handle (VkSwapchainKHR).
         */
        virtual VkSwapchainKHR Get() = 0;

        /**
         * @brief Constructs or rebuilds the swapchain with the given parameters.
         *
         * This method is responsible for creating or recreating the Vulkan swapchain
         * using a given surface, frame dimensions, and an optional old swapchain handle.
         *
         * @param surface The Vulkan surface on which the swapchain will be created.
         * @param frame The dimensions of the swapchain's images, specified as a `VkExtent2D` structure.
         * @param oldSwapChain An optional handle to the old swapchain, which can be used for efficient
         *        chain replacement; pass `VK_NULL_HANDLE` if no previous swapchain exists.
         */
        virtual void Build(VkSurfaceKHR surface, VkExtent2D frame,
                           VkSwapchainKHR oldSwapChain) = 0;

        /**
         * @brief Recreates the swap chain with a new width and height.
         *
         * This pure virtual function is responsible for reinitializing the swap chain
         * with the specified dimensions. It must be implemented by derived classes
         * to handle changes in the render target size.
         *
         * @param width The new width of the swap chain.
         * @param height The new height of the swap chain.
         */
        virtual void RecreateSwapChain(uint32_t width, uint32_t height) = 0;


        VkExtent2D swapChainExtent{};
        VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;
        std::vector<std::shared_ptr<ImageViews::ImageView> > _swapChainImageViews;
        std::vector<VkImage> _swapChainImages;
    };
}

#endif // ENTROPY_ISWAPCHAIN_H
