#pragma once

#include <config.hpp>
#include <iostream>
#include <set>
#include <string>
#include <vulkan/vulkan.hpp>

#include <graphics/devices/physical_device.hpp>
#include <graphics/queuefamilies/queuefamily.hpp>
#include <graphics/surfaces/surface.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::QueueFamilies;
using namespace Entropy::Graphics::Devices;

namespace Entropy {
namespace Graphics {
namespace Devices {
class LogicalDevice : public Service {
public:
  LogicalDevice(std::shared_ptr<PhysicalDevice> physicalDevice,
                std::shared_ptr<WindowSurface> surface);
  LogicalDevice(std::shared_ptr<PhysicalDevice> physicalDevice);
  inline VkDevice Get() { return _logicalDevice; };
  VkQueue GetGraphicQueue() { return _graphicsQueue; };
  VkQueue GetPresentQueue() { return _presentQueue; };

  uint32_t queueFamiliy = 0;

private:
  VkDevice _logicalDevice;
  VkQueue _graphicsQueue = VK_NULL_HANDLE;
  VkQueue _presentQueue = VK_NULL_HANDLE;
};
} // namespace Devices
} // namespace Graphics
} // namespace Entropy
