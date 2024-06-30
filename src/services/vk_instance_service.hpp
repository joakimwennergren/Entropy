#pragma once

#include <graphics/vulkan/instances/vk_instance.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::Vulkan::Instances;

namespace Entropy {
namespace Graphics {
namespace Services {

struct VulkanInstanceService : kgr::single_service<VulkanInstance> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
