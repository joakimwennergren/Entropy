#pragma once

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

#include <graphics/vulkan/vulkan_backend.hpp>



namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace CommandPools {
class CommandPool{
public:
  CommandPool(VulkanBackend vbe) : _vkBackend{vbe}
  {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = _vkBackend.logicalDevice.queueFamiliy;

    if (vkCreateCommandPool(_vkBackend.logicalDevice.Get(), &poolInfo, nullptr,
                            &_commandPool) != VK_SUCCESS) {
      spdlog::error("Failed to create command pool.");
    }
  }
  inline VkCommandPool Get() { return _commandPool; };

private:
  VulkanBackend _vkBackend;
  VkCommandPool _commandPool = VK_NULL_HANDLE;
};
} // namespace CommandPools
} // namespace Graphics
} // namespace Entropy
}