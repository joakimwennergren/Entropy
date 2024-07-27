#pragma once
#include "../../graphics/vulkan/descriptorsetlayouts/descriptorsetlayout.hpp"

using namespace Entropy::Graphics::Vulkan;
using namespace Entropy::Graphics::Vulkan::DescriptorsetLayouts;

namespace Entropy
{
  namespace Factories
  {
    namespace Vulkan
    {

      struct DescriptorSetLayoutFactory
      {

        DescriptorSetLayoutFactory() {}

        DescriptorsetLayout
        CreateLayout(std::vector<VkDescriptorSetLayoutBinding> layoutBindings,
                     std::vector<VkDescriptorBindingFlagsEXT> bindingFlags)
        {
          return DescriptorsetLayout(layoutBindings, bindingFlags);
        }

      private:
      };

    } // namespace Vulkan
  } // namespace Factories
} // namespace Entropy