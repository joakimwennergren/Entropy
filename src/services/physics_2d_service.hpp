
#pragma once

#include "kangaru/service.hpp"
#include "physics/2d/physics2d.hpp"
#include "services/entity_factory_service.hpp"
#include "services/logical_device_service.hpp"
#include "services/physical_device_service.hpp"
#include "services/vk_instance_service.hpp"
#include <graphics/vulkan/memory/allocator.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::Vulkan::Memory;

namespace Entropy {
namespace Graphics {
namespace Services {

struct Physics2DService
    : kgr::single_service<Physics::Physics2D,
                          kgr::dependency<EntityFactoryService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
