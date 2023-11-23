#include "vulkancontext.hpp"

using namespace Entropy::Global;

VulkanContext *VulkanContext::pinstance_{nullptr};
std::mutex VulkanContext::mutex_;

VulkanContext *VulkanContext::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new VulkanContext();
    }
    return pinstance_;
}