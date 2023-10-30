#include "synchronizer.hpp"

using namespace Symbios::Graphics::Synchronization;

Synchronizer::Synchronizer(unsigned int numObjects)
{
    this->numObjects = numObjects;

    // Get Vulkan ctx
    _context = Global::VulkanContext::GetInstance()->GetVulkanContext();

    _imageSemaphores.resize(numObjects);
    _renderFinishedSemaphores.resize(numObjects);
    _fences.resize(numObjects);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < numObjects; i++)
    {
        if (vkCreateSemaphore(_context->GetLogicalDevice(), &semaphoreInfo, nullptr, &_imageSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(_context->GetLogicalDevice(), &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(_context->GetLogicalDevice(), &fenceInfo, nullptr, &_fences[i]) != VK_SUCCESS)
        {

            PLOG_ERROR << "failed to create synchronization objects!";
            exit(EXIT_FAILURE);
        }
    }

}

Synchronizer::~Synchronizer()
{
    vkDeviceWaitIdle(_context->GetLogicalDevice());

    for (size_t i = 0; i < this->numObjects; i++)
    {
        vkDestroySemaphore(_context->GetLogicalDevice(), _imageSemaphores[i], nullptr);
        vkDestroySemaphore(_context->GetLogicalDevice(), _renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(_context->GetLogicalDevice(), _fences[i], nullptr);
    }
}