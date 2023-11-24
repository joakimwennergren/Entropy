#include "synchronizer.hpp"

using namespace Entropy::Graphics::Synchronization;

Synchronizer::Synchronizer(unsigned int numObjects)
{
    this->numObjects = numObjects;

    VulkanContext *vkContext = VulkanContext::GetInstance();

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
        if (vkCreateSemaphore(vkContext->logicalDevice, &semaphoreInfo, nullptr, &_imageSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(vkContext->logicalDevice, &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(vkContext->logicalDevice, &fenceInfo, nullptr, &_fences[i]) != VK_SUCCESS)
        {

            PLOG_ERROR << "failed to create synchronization objects!";
            exit(EXIT_FAILURE);
        }
    }
}

Synchronizer::~Synchronizer()
{
    VulkanContext *vkContext = VulkanContext::GetInstance();

    vkDeviceWaitIdle(vkContext->logicalDevice);

    for (size_t i = 0; i < this->numObjects; i++)
    {
        vkDestroySemaphore(vkContext->logicalDevice, _imageSemaphores[i], nullptr);
        vkDestroySemaphore(vkContext->logicalDevice, _renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(vkContext->logicalDevice, _fences[i], nullptr);
    }
}