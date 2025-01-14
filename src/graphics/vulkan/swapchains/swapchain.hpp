#ifndef ENTROPY_SWAPCHAIN_H
#define ENTROPY_SWAPCHAIN_H

#include <vulkan/vulkan.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/queuefamilies/queuefamily.hpp>
#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/devices/iphysical_device.hpp>
#include <graphics/vulkan/utilities/helpers.hpp>

#include "iswapchain.hpp"

namespace Entropy::Graphics::Vulkan::SwapChains {
  /**
   * @brief Represents the details of swap chain support for a Vulkan physical device.
   *
   * This class contains information about the swap chain capabilities, formats,
   * and present modes supported by a Vulkan physical device. It is used to assess
   * and configure the swap chain for rendering.
   */
  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities{};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  struct SwapChain final : ServiceBase<ISwapchain> {
    /**
     * @brief Constructs a SwapChain object and initializes Vulkan device-specific members.
     *
     * This constructor retrieves essential Vulkan device interfaces via a ServiceLocator
     * instance and sets up the initial swap chain image format and extent.
     *
     * @return A newly constructed SwapChain object.
     */
    SwapChain();

    /**
     * @brief Builds the Vulkan swap chain for a specific surface, frame dimensions, and optionally reuses an old swap chain.
     *
     * This function creates and configures the Vulkan swap chain based on the capabilities
     * of the specified physical device and surface. It determines the optimal image format,
     * presentation mode, and other parameters necessary for rendering. It also handles
     * the recreation of the swap chain if an old one is provided.
     *
     * @param surface The Vulkan surface to associate the swap chain with.
     * @param frame The desired frame extent, specifying the width and height of the render area.
     * @param oldSwapChain An optional handle to the existing swap chain to be replaced, if applicable.
     */
    void Build(VkSurfaceKHR surface, VkExtent2D frame,
               VkSwapchainKHR oldSwapChain) override;

    /**
     * @brief Recreates the Vulkan swap chain with the specified dimensions and application state.
     *
     * This method updates the swap chain image format, extent, and clears existing swap chain images and image views.
     * If the application state is true, it calls the Build function to rebuild the swap chain.
     *
     * @param width The new width for the swap chain extent.
     * @param height The new height for the swap chain extent.
     * @param app The application state that determines whether to rebuild the swap chain.
     */
    void RecreateSwapChain(uint32_t width, uint32_t height) override;

    /**
     * @brief Selects the most suitable surface format for the swap chain.
     *
     * This method chooses the best surface format from the available formats
     * based on predefined preferences, such as color format and color space.
     * If no preferred format is found, it defaults to the first available format.
     *
     * @param availableFormats A vector of available VkSurfaceFormatKHR objects
     *                         describing supported surface formats and color spaces.
     * @return The selected VkSurfaceFormatKHR that best matches the preferred criteria or
     *         defaults to the first available format.
     */
    static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats);

    /**
     * @brief Selects the optimal present mode for the swap chain.
     *
     * This method evaluates the available present modes and selects the optimal mode
     * based on the desired rendering behavior. The preferred mode is VK_PRESENT_MODE_MAILBOX_KHR
     * for its low latency, but if unavailable, VK_PRESENT_MODE_FIFO_KHR is used as the fallback
     * mode to ensure compatibility.
     *
     * @param availablePresentModes A reference to a vector containing the present modes
     *        supported by the swap chain.
     * @return The selected VkPresentModeKHR that is most suitable for rendering.
     */
    static VkPresentModeKHR ChooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);

    /**
     * @brief Chooses the optimal swap extent for the Vulkan swap chain.
     *
     * Determines the dimensions of the swap extent for a Vulkan surface based on
     * the provided surface capabilities and the frame dimensions. The swap extent
     * is used to define the resolution at which the images will be rendered.
     *
     * @param capabilities The surface capabilities defining the supported
     *        extents, including minimum, maximum, and current extents.
     * @param frame The desired frame dimensions, typically the resolution
     *        of the window or rendering area.
     * @return The selected VkExtent2D that matches the optimal swap extent
     *         based on constraints and desired frame size.
     */
    static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
                                       VkExtent2D frame);

    /**
     * @brief Queries the swap chain support details for a given Vulkan physical device and surface.
     *
     * This function retrieves the capabilities, formats, and present modes supported
     * by the specified Vulkan physical device and surface, which are used to configure
     * the swap chain for rendering.
     *
     * @param device The Vulkan physical device to query for swap chain support.
     * @param surface The Vulkan surface associated with the swap chain.
     * @return A SwapChainSupportDetails structure containing the capabilities, formats,
     *         and present modes supported by the given device and surface.
     */
    static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device,
                                                         VkSurfaceKHR surface);

    /**
     * @brief Retrieves the Vulkan swap chain handle.
     *
     * This method returns the Vulkan swap chain (VkSwapchainKHR) associated
     * with the current instance. The swap chain is used for rendering and
     * presenting images to the screen.
     *
     * @return The Vulkan swap chain handle (VkSwapchainKHR).
     */
    VkSwapchainKHR Get() override {
      return _swapChain;
    };

  private:
    VkSurfaceKHR _surface = VK_NULL_HANDLE;
    VkSwapchainKHR _swapChain = VK_NULL_HANDLE;
    VkSwapchainKHR _oldSwapChain = VK_NULL_HANDLE;
    std::shared_ptr<IPhysicalDevice> _physicalDevice;
    std::shared_ptr<ILogicalDevice> _logicalDevice;
  };
} // namespace Entropy::Graphics::Vulkan::SwapChains

#endif // ENTROPY_SWAPCHAIN_H
