#pragma once

#include <graphics/devices/device.hpp>
#include <kangaru/kangaru.hpp>

namespace Entropy {
namespace Graphics {
namespace Services {
struct PhysicalDeviceService : kgr::single_service<PhysicalDevice> {};
} // namespace Services
} // namespace Graphics
} // namespace Entropy
