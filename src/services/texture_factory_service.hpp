
#pragma once

#include <kangaru/kangaru.hpp>
#include <factories/vulkan/texturefactory.hpp>
#include <services/queue_sync_service.hpp>
#include <services/allocator_service.hpp>

using namespace Entropy::Factories::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Services {

struct TextureFactoryService
    : kgr::single_service<TextureFactory, kgr::dependency<VulkanBackendService, QueueSyncService, AllocatorService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
