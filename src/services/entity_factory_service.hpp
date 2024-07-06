
#pragma once

#include "services/buffer_factory_service.hpp"
#include "services/vulkan_backend_service.hpp"
#include <factories/entityfactory.hpp>
#include <kangaru/kangaru.hpp>
#include <scripting/lua.hpp>
#include <services/scripting_service.hpp>
#include <services/world_service.hpp>

using namespace Entropy::Factories;

namespace Entropy {
namespace Graphics {
namespace Services {

struct EntityFactoryService
    : kgr::single_service<EntityFactory,
                          kgr::dependency<WorldService, VulkanBackendService,
                                          BufferFactoryService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
