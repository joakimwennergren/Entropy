#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>
#include <flecs/flecs.h>


namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Synchronization {
class QueueSync{
public:
  std::vector<VkCommandBuffer> commandBuffers;
};
} // namespace Synchronization
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy