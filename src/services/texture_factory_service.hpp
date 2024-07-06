
#pragma once

#include "services/buffer_factory_service.hpp"
#include "services/command_pool_service.hpp"
#include <factories/vulkan/texturefactory.hpp>
#include <kangaru/kangaru.hpp>
#include <services/allocator_service.hpp>
#include <services/queue_sync_service.hpp>

using namespace Entropy::Factories::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Services {

struct TextureFactoryService
    : kgr::single_service<
          TextureFactory,
          kgr::dependency<VulkanBackendService, QueueSyncService,
                          AllocatorService, BufferFactoryService,
                          CommandPoolService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
