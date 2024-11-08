#pragma once

#include <servicelocators/servicelocator.hpp>
#include <vulkan/vulkan.hpp>

struct IDescriptorPool : IService {
    /**
     * @brief Virtual destructor for the IDescriptorPool interface.
     *
     * This destructor ensures that derived classes can clean up resources
     * appropriately when the pool is destroyed.
     */
    ~IDescriptorPool() override = default;

    virtual VkDescriptorPool Get() = 0;
};
