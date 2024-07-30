#pragma once

#include <servicelocators/servicelocator.hpp>

class ICommandPool : public IService
{
public:
    virtual ~ICommandPool() = default;
    virtual VkCommandPool Get() = 0;
};