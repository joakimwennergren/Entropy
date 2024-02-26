#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>
#include <string>
#include <config.hpp>

#include <graphics/devices/logical_device.hpp>
#include <graphics/descriptorsetlayouts/descriptorsetlayout.hpp>
#include <graphics/descriptorpools/descriptorpool.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::QueueFamilies;
using namespace Entropy::Graphics::Devices;
using namespace Entropy::Graphics::DescriptorsetLayouts;
using namespace Entropy::Graphics::DescriptorPools;

namespace Entropy
{
    namespace Graphics
    {
        namespace Descriptorsets
        {
            class Descriptorset : public Service
            {
            public:
                Descriptorset(std::shared_ptr<LogicalDevice> logicalDevice, std::shared_ptr<DescriptorPool> pool, std::shared_ptr<DescriptorsetLayout> layout);

                inline std::vector<VkDescriptorSet> Get() { return _descriptorSets; };

            private:
                std::vector<VkDescriptorSet> _descriptorSets;
            };
        }
    }
}
