#pragma once

#include <graphics/devices/device.hpp>
#include <kangaru/kangaru.hpp>

namespace Entropy {
namespace Graphics {
namespace Services {
struct LogicalDeviceService : kgr::single_service<LogicalDevice> {};
} // namespace Services
} // namespace Graphics
} // namespace Entropy
