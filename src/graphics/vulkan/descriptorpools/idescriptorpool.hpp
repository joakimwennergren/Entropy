#ifndef __IDESCRIPTORPOOL_HPP
#define __IDESCRIPTORPOOL_HPP

#include <servicelocators/servicelocator.hpp>
#include <vulkan/vulkan.hpp>

class IDescriptorPool : public IService {
public:
  virtual ~IDescriptorPool() = default;
  virtual VkDescriptorPool Get() = 0;
};

#endif /* __IDESCRIPTORPOOL_HPP */
