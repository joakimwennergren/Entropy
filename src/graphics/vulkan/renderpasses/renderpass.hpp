#pragma once

#include <graphics/devices/logical_device.hpp>
#include <graphics/imageviews/imageview.hpp>
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>
#include <servicelocators/servicelocator.hpp>
#include <vulkan/vulkan.hpp>

#include "spdlog/spdlog.h"

using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Vulkan::Swapchains;
using namespace Entropy::Graphics::Devices;
using namespace Entropy::Graphics::ImageViews;
using namespace Entropy::Graphics::Vulkan::Memory;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace RenderPasses {
class RenderPass {
public:
  RenderPass(std::shared_ptr<ServiceLocator> serviceLocator);
  ~RenderPass();

  void Begin(std::shared_ptr<CommandBuffer> commandBuffer,
             uint32_t imageIndex) const;
  void End(std::shared_ptr<CommandBuffer> commandBuffer) const;

  void RecreateFrameBuffers();
  void RecreateDepthBuffer();

  inline VkRenderPass GetRenderPass() const { return this->_renderPass; };

private:
  void CreateFramebuffers();

  VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);
  VkFormat FindDepthFormat();

  void CreateImage(uint32_t width, uint32_t height, VkFormat format,
                   VkImageTiling tiling, VkImageUsageFlags usage);
  uint32_t findMemoryType(uint32_t typeFilter,
                          VkMemoryPropertyFlags properties);
  VkRenderPass _renderPass;
  std::vector<VkFramebuffer> _swapChainFramebuffers;

  std::shared_ptr<Swapchain> _swapChain;
  std::shared_ptr<LogicalDevice> _logicalDevice;
  std::shared_ptr<PhysicalDevice> _physicalDevice;
  std::shared_ptr<ServiceLocator> _serviceLocator;

  VkImage _depthImage = VK_NULL_HANDLE;
  VkImageView depthImageView;
  VmaAllocation _allocation;
};
} // namespace RenderPasses
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
