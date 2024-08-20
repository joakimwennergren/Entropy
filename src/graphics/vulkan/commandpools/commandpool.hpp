#ifndef __COMMANDPOOL_HPP
#define __COMMANDPOOL_HPP

#include "icommandpool.hpp"

#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace CommandPools {
class CommandPool : public ServiceBase<ICommandPool> {
public:
  CommandPool() {
    ServiceLocator *sl = ServiceLocator::GetInstance();
    _logicalDevice = sl->getService<ILogicalDevice>();

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = _logicalDevice->queueFamiliy;

    VK_CHECK(vkCreateCommandPool(_logicalDevice->Get(), &poolInfo, nullptr,
                                 &_commandPool));
  }

  ~CommandPool() {
    // vkDestroyCommandPool(_logicalDevice->Get(), _commandPool, nullptr);
  }

  inline VkCommandPool Get() { return _commandPool; };

private:
  VkCommandPool _commandPool = VK_NULL_HANDLE;
  std::shared_ptr<ILogicalDevice> _logicalDevice;
};
} // namespace CommandPools
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy

#endif /* __COMMANDPOOL_HPP */
