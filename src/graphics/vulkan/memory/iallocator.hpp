#ifndef ENTROPY_IALLOCATOR_H
#define ENTROPY_IALLOCATOR_H


#include <vk_mem_alloc.h>
#include <servicelocators/servicelocator.hpp>

/**
 * @brief Interface representing a Vulkan memory allocator service.
 *
 * The IAllocator interface provides an abstraction for managing Vulkan
 * Memory Allocator (VMA) instances. It extends the IService interface
 * and is intended to be implemented by classes that handle GPU memory
 * allocation efficiently in Vulkan applications.
 */
namespace Entropy::Graphics::Vulkan::Memory {
    struct IAllocator : IService {
        /**
         * @brief Destructor for the IAllocator interface.
         *
         * This is a defaulted destructor that ensures proper cleanup
         * of resources for any object that inherits from the IAllocator
         * interface. It also ensures that the destructor is virtual,
         * allowing derived class destructors to be invoked correctly
         * during object destruction.
         */
        ~IAllocator() override = default;

        /**
         * @brief Retrieves the Vulkan Memory Allocator (VMA) instance.
         *
         * This pure virtual function provides access to the VMA instance associated
         * with the class that implements this interface. The returned instance is
         * required for managing GPU memory allocations efficiently within the Vulkan
         * API abstraction using VMA.
         *
         * @return A `VmaAllocator` representing the Vulkan Memory Allocator instance.
         */
        virtual VmaAllocator Get() = 0;
    };
}

#endif //ENTROPY_IALLOCATOR_H
