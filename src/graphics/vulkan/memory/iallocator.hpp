#pragma once

#include "vk_mem_alloc.h"
#include <servicelocators/servicelocator.hpp>

struct IAllocator : public IService
{
    virtual ~IAllocator() = default;
    virtual VmaAllocator Get() = 0;
};