#ifndef __ICOMMANDPOOL_HPP
#define __ICOMMANDPOOL_HPP

#include <servicelocators/servicelocator.hpp>
#include <vulkan/vulkan.hpp>

class ICommandPool : public IService {
public:
  virtual ~ICommandPool() = default;
  virtual VkCommandPool Get() = 0;
};

#endif /* __ICOMMANDPOOL_HPP */
