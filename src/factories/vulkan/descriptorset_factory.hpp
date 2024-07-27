#pragma once
#include "../../graphics/vulkan/descriptorsets/descriptorset.hpp"

using namespace Entropy::Graphics::Vulkan;
using namespace Entropy::Graphics::Vulkan::Descriptorsets;

namespace Entropy
{
  namespace Factories
  {
    namespace Vulkan
    {

      struct DescriptorSetFactory
      {

        DescriptorSetFactory()
        {
        }

        Descriptorset CreateDescriptorSet(DescriptorsetLayout layout)
        {
          return Descriptorset(layout);
        }

      private:
      };

    } // namespace Drawables
  } // namespace Vulkan
} // namespace Graphics