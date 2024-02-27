#pragma once

#include "spdlog/spdlog.h"
#include <services/service.hpp>
#include <graphics/instances/vk_instance.hpp>
#include <graphics/devices/physical_device.hpp>
#include <graphics/devices/logical_device.hpp>
#include <servicelocators/servicelocator.hpp>

#include "vk_mem_alloc.h"

using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Devices;
using namespace Entropy::Graphics::Instances;

namespace Entropy
{
    namespace Graphics
    {
        namespace Memory
        {
            class Allocator : public Service
            {
            public:
                Allocator(std::shared_ptr<VulkanInstance> instance, std::shared_ptr<LogicalDevice> logicalDevice, std::shared_ptr<PhysicalDevice> phyiscalDevice);
                ~Allocator(); 

                inline VmaAllocator Get() { return _allocator;};

            private:
                VmaAllocator _allocator;
            };
        }
    }
}
