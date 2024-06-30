#pragma once

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

#include <graphics/devices/logical_device.hpp>
#include <graphics/devices/physical_device.hpp>
#include <graphics/vulkan/queuefamilies/queuefamily.hpp>
#include <graphics/vulkan/surfaces/surface.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::QueueFamilies;
using namespace Entropy::Graphics::Devices;
using namespace Entropy::Services;

namespace Entropy {
namespace Graphics {
namespace CommandPools {
class CommandPool : public Service {
public:
  CommandPool() = default;
  CommandPool(std::shared_ptr<LogicalDevice> logicalDevice,
              std::shared_ptr<PhysicalDevice> physicalDevice,
              std::shared_ptr<WindowSurface> surface);
  CommandPool(std::shared_ptr<LogicalDevice> logicalDevice,
              std::shared_ptr<PhysicalDevice> physicalDevice);
  inline VkCommandPool Get() { return _commandPool; };

private:
  VkCommandPool _commandPool = VK_NULL_HANDLE;
};
} // namespace CommandPools
} // namespace Graphics
} // namespace Entropy