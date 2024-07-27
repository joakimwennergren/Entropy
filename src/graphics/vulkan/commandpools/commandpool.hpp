#pragma once

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

#include <graphics/vulkan/devices/ilogical_device.hpp>
#include "icommandpool.hpp"

namespace Entropy
{
  namespace Graphics
  {
    namespace Vulkan
    {
      namespace CommandPools
      {
        class CommandPool : public ServiceBase<ICommandPool>
        {
        public:
          CommandPool()
          {
            ServiceLocator *sl = ServiceLocator::GetInstance();
            auto logicalDevice = sl->getService<ILogicalDevice>();

            VkCommandPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            poolInfo.queueFamilyIndex = logicalDevice->queueFamiliy;

            if (vkCreateCommandPool(logicalDevice->Get(), &poolInfo, nullptr,
                                    &_commandPool) != VK_SUCCESS)
            {
              spdlog::error("Failed to create command pool.");
            }
          }
          inline VkCommandPool Get() { return _commandPool; };

        private:
          VkCommandPool _commandPool = VK_NULL_HANDLE;
        };
      } // namespace CommandPools
    } // namespace Graphics
  } // namespace Entropy
}