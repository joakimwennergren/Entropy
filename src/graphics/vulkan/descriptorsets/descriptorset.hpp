#ifndef __DESCRIPTORSET_HPP
#define __DESCRIPTORSET_HPP

#include <config.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

#include <graphics/vulkan/descriptorpools/idescriptorpool.hpp>
#include <graphics/vulkan/descriptorsetlayouts/descriptorsetlayout.hpp>
#include <graphics/vulkan/devices/ilogical_device.hpp>

using namespace Entropy::Graphics::Vulkan::DescriptorsetLayouts;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Descriptorsets {
class Descriptorset {
public:
  Descriptorset(DescriptorsetLayout layout) {
    ServiceLocator *sl = ServiceLocator::GetInstance();
    auto logicalDevice = sl->getService<ILogicalDevice>();
    auto descriptorPool = sl->getService<IDescriptorPool>();

    std::vector<VkDescriptorSetLayout> layouts(MAX_CONCURRENT_FRAMES_IN_FLIGHT,
                                               layout.Get());

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool->Get();
    allocInfo.descriptorSetCount = MAX_CONCURRENT_FRAMES_IN_FLIGHT;
    allocInfo.pSetLayouts = layouts.data();

    _descriptorSets.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);

    VK_CHECK(vkAllocateDescriptorSets(logicalDevice->Get(), &allocInfo,
                                      _descriptorSets.data()));
  }

  inline std::vector<VkDescriptorSet> Get() { return _descriptorSets; };

private:
  std::vector<VkDescriptorSet> _descriptorSets;
};
} // namespace Descriptorsets
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy

#endif /* __DESCRIPTORSET_HPP */
