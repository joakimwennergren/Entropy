#pragma once

#include "graphics/devices/logical_device.hpp"
#include <graphics/devices/physical_device.hpp>

using namespace Entropy::Graphics::Devices;

namespace Entropy {
namespace Graphics {
namespace Devices {
struct Device {
  PhysicalDevice physicalDevice;
};
} // namespace Devices
} // namespace Graphics
} // namespace Entropy