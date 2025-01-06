#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Synchronization {
class QueueSync {
public:
  std::vector<VkCommandBuffer> commandBuffers;
};
} // namespace Synchronization
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy