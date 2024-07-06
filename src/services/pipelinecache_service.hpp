
#pragma once

#include "services/logical_device_service.hpp"
#include <graphics/vulkan/pipelinecaches/pipelinecache.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::Vulkan::Caches;

namespace Entropy {
namespace Graphics {
namespace Services {

struct PipelineCacheService
    : kgr::single_service<PipelineCache,
                          kgr::dependency<LogicalDeviceService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
