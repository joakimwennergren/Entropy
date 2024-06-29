
#pragma once

#include "services/device_service.hpp"
#include <graphics/swapchains/swapchain.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::Swapchains;

namespace Entropy {
namespace Graphics {
namespace Services {

struct SwapchainService
    : kgr::single_service<Swapchain, kgr::dependency<DeviceService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
