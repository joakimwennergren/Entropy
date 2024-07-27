#pragma once

#include <servicelocators/servicelocator.hpp>

class IDescriptorPool : public IService
{
public:
    virtual ~IDescriptorPool() = default;
    virtual VkDescriptorPool Get() = 0;
};