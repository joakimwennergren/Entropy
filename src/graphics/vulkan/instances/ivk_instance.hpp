#pragma once

#include <vulkan/vulkan.hpp>
#include <servicelocators/servicelocator.hpp>

/**
 * @brief Represents a Vulkan instance used to manage the Vulkan API state.
 *
 * This interface provides functionality to create, configure, and manage
 * a Vulkan instance. The Vulkan instance serves as the connection between
 * the application and the Vulkan library, initializing the API, setting up
 * extensions, and validating supported layers.
 *
 * Implementations of this interface typically allow querying instance-specific
 * details, interacting with Vulkan-supported layers and extensions, and serving
 * as the foundational object for further Vulkan operations.
 *
 * It ensures proper setup for rendering tasks and the efficient use of Vulkan
 * resources, acting as an entry point for graphics and compute functionalities.
 */
struct IVulkanInstance : IService {
    /**
     * @brief Virtual destructor for the IVulkanInstance interface.
     *
     * Ensures proper cleanup of resources associated with the IVulkanInstance
     * implementation. When a derived class is destroyed, this destructor ensures
     * that derived class destructors are invoked correctly, followed by the cleanup
     * handled by IVulkanInstance.
     *
     * Marked as default to indicate that the destructor's behavior relies on the compiler-
     * generated implementation, ensuring optimal and safe resource management for
     * derived classes.
     */
    ~IVulkanInstance() override = default;

    /**
     * @brief Retrieves the value or data associated with the current object or operation.
     *
     * This method provides access to the underlying value or state managed by the object,
     * typically returning data relevant to the object's purpose or context. It ensures
     * encapsulated access and may include additional logic before delivering the result.
     *
     * The behavior and return type depend on the specific implementation and use case.
     *
     * @return The value or data associated with the operation or object.
     */
    virtual VkInstance Get() = 0;
};
