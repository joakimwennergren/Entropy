
#pragma once

#include <factories/vulkan/bufferfactory.hpp>
#include <kangaru/kangaru.hpp>
#include <services/vulkan_backend_service.hpp>

using namespace Entropy::Factories::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Services {

struct BufferFactoryService
    : kgr::single_service<BufferFactory,
                          kgr::dependency<VulkanBackendService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
