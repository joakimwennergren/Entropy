#pragma once

#include <servicelocators/servicelocator.hpp>
#include <vulkan/vulkan.hpp>

class IDescriptorPool : public IService {
public:
  virtual ~IDescriptorPool() = default;
  virtual VkDescriptorPool Get() = 0;
};
