#pragma once

#include <graphics/vulkan/synchronization/queuesync.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::Vulkan::Synchronization;

namespace Entropy {
namespace Graphics {
namespace Services {
struct QueueSyncService
    : kgr::single_service<QueueSync> {};
} // namespace Services
} // namespace Graphics
} // namespace Entropy
