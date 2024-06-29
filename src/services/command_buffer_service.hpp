
#pragma once

#include "kangaru/service.hpp"
#include "services/allocator_service.hpp"
#include "services/command_pool_service.hpp"
#include "services/device_service.hpp"
#include <graphics/commandbuffers/commandbuffer.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::CommandBuffers;

namespace Entropy {
namespace Graphics {
namespace Services {

struct CommandBufferService
    : kgr::service<CommandBuffer,
                   kgr::dependency<DeviceService, CommandPoolService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
