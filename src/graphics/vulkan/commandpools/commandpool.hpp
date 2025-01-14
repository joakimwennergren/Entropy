#pragma once

#include "icommandpool.hpp"

#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/utilities/helpers.hpp>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

namespace Entropy::Graphics::Vulkan::CommandPools {
  struct CommandPool final : ServiceBase<ICommandPool> {
    /**
     * Constructs a CommandPool object and initializes a Vulkan command pool.
     *
     * This constructor retrieves the logical device from the ServiceLocator,
     * sets up the VkCommandPoolCreateInfo structure, and creates a Vulkan command pool.
     * The command pool allows for command buffer management and supports the reset command buffer operation.
     *
     * @return Initialized CommandPool object.
     */
    CommandPool() {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      _logicalDevice = sl->getService<ILogicalDevice>();

      VkCommandPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
      poolInfo.queueFamilyIndex = _logicalDevice->queueFamiliy;

      VK_CHECK(vkCreateCommandPool(_logicalDevice->Get(), &poolInfo, nullptr,
        &_commandPool));
    }

    /**
     * Destroys the CommandPool object and cleans up the Vulkan command pool.
     *
     * This destructor releases the Vulkan command pool associated with the logical device.
     * It ensures that the Vulkan command pool is properly destroyed before the CommandPool
     * object is deallocated.
     */
    ~CommandPool() override {
      vkDestroyCommandPool(_logicalDevice->Get(), _commandPool, nullptr);
    }

    VkCommandPool Get() override { return _commandPool; }

  private:
    VkCommandPool _commandPool = VK_NULL_HANDLE;
    std::shared_ptr<ILogicalDevice> _logicalDevice;
  };
} // namespace Entropy::Graphics::Vulkan::CommandPools
