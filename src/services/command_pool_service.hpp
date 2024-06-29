
#pragma once

#include "kangaru/service.hpp"
#include "services/allocator_service.hpp"
#include "services/device_service.hpp"
#include <graphics/commandbuffers/commandbuffer.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::CommandPools;

namespace Entropy {
namespace Graphics {
namespace Services {

struct CommandPoolService
    : kgr::service<CommandPool, kgr::dependency<DeviceService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
