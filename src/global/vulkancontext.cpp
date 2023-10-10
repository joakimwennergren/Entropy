#include "vulkancontext.hpp"

using namespace Symbios::Global;

/**
 * Static methods should be defined outside the class.
 */

VulkanContext *VulkanContext::pinstance_{nullptr};
std::mutex VulkanContext::mutex_;

/**
 * The first time we call GetInstance we will lock the storage location
 *      and then we make sure again that the variable is null and then we
 *      set the value. RU:
 */
VulkanContext *VulkanContext::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new VulkanContext();
    }
    return pinstance_;
}