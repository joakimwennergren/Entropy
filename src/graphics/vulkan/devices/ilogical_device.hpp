#pragma once

#include <vulkan/vulkan.hpp>
#include <servicelocators/servicelocator.hpp>

struct ILogicalDevice : public IService {
    ~ILogicalDevice() override = default;

    virtual VkDevice Get() = 0;

    virtual VkQueue GetGraphicQueue() = 0;

    // QueueFamily index
    uint32_t queueFamiliy = 0;
};
