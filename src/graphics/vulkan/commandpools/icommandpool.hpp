#pragma once

#include <servicelocators/servicelocator.hpp>
#include <vulkan/vulkan.hpp>

struct ICommandPool : IService {
  ~ICommandPool() override = default;

  virtual VkCommandPool Get() = 0;
};
