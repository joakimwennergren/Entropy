
#pragma once

#include <kangaru/kangaru.hpp>
#include <factories/vulkan/bufferfactory.hpp>

using namespace Entropy::Factories::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Services {

struct BufferFactoryService
    : kgr::single_service<BufferFactory, kgr::dependency<VulkanBackendService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
