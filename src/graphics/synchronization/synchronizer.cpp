#include "synchronizer.hpp"

using namespace Entropy::Graphics::Synchronization;

Synchronizer::Synchronizer(unsigned int numObjects, std::shared_ptr<ServiceLocator> serviceLocator)
{

    assert(numObjects != 0);
    assert(serviceLocator != nullptr);

    // Get required depenencies
    _logicalDevice = serviceLocator->GetService<LogicalDevice>();
    _numObjects = numObjects;

    _imageSemaphores.resize(numObjects);
    _renderFinishedSemaphores.resize(numObjects);
    _fences.resize(numObjects);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < _numObjects; i++)
    {
        if (vkCreateSemaphore(_logicalDevice->Get(), &semaphoreInfo, nullptr, &_imageSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(_logicalDevice->Get(), &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(_logicalDevice->Get(), &fenceInfo, nullptr, &_fences[i]) != VK_SUCCESS)
        {
            spdlog::error("Could not create synchronizer objects.");
        }
    }
}

Synchronizer::~Synchronizer()
{
    vkDeviceWaitIdle(_logicalDevice->Get());

    for (size_t i = 0; i < _numObjects; i++)
    {
        vkDestroySemaphore(_logicalDevice->Get(), _imageSemaphores[i], nullptr);
        vkDestroySemaphore(_logicalDevice->Get(), _renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(_logicalDevice->Get(), _fences[i], nullptr);
    }
}