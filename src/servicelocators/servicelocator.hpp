#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <typeindex>
#include <type_traits>
#include <iostream>

/// Base class for all services.
class IService
{
public:
    virtual ~IService() = default;

    /// Virtual function to get the base type index of the service.
    /// Should be overridden in each service interface to return the typeid of the base interface.
    virtual std::type_index getTypeIndex() const = 0;
};

/// Specific service base class.
/// Inherit from this instead of IService directly for services with multiple implementations.
template <typename T>
class ServiceBase : public T
{
public:
    /// Returns the type index of the base interface.
    std::type_index getTypeIndex() const override
    {
        return std::type_index(typeid(T));
    }
};

/// Service Locator class.
class ServiceLocator
{
private:
    std::map<std::type_index, std::shared_ptr<IService>> services;
    mutable std::mutex mutex;

protected:
    ServiceLocator()
    {
    }
    static ServiceLocator *_instance;

public:
    /**
     * Singletons should not be cloneable.
     */
    ServiceLocator(ServiceLocator &other) = delete;

    /**
     * Singletons should not be assignable.
     */
    void operator=(const ServiceLocator &) = delete;

    /**
     * Static methods should be defined outside the class.
     */
    static ServiceLocator *GetInstance()
    {
        /**
         * This is a safer way to create an instance. instance = new Singleton is
         * dangeruous in case two instance threads wants to access at the same time
         */
        if (_instance == nullptr)
        {
            _instance = new ServiceLocator();
        }
        return _instance;
    }

    /// Registers a service with the service locator.
    /// @tparam T The service implementation type.
    /// @param service Shared pointer to the service instance.
    template <typename T>
    void registerService(std::shared_ptr<T> service)
    {
        static_assert(std::is_base_of<IService, T>::value, "T must inherit from IService");
        std::lock_guard<std::mutex> lock(mutex);
        std::type_index typeIndex = service->getTypeIndex(); // Use base class type index
        if (services.find(typeIndex) != services.end())
        {
            throw std::runtime_error("Service already registered");
        }
        services[typeIndex] = service;
    }

    /// Unregisters a service from the service locator.
    /// @tparam T The base service type.
    template <typename T>
    void unregisterService()
    {
        std::lock_guard<std::mutex> lock(mutex);
        std::type_index typeIndex = std::type_index(typeid(T));
        if (services.find(typeIndex) == services.end())
        {
            throw std::runtime_error("Service not registered");
        }
        services.erase(typeIndex);
    }

    /// Gets a service from the service locator.
    /// @tparam T The base service type.
    /// @return Shared pointer to the requested service instance.
    template <typename T>
    std::shared_ptr<T> getService() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        std::type_index typeIndex = std::type_index(typeid(T));
        auto it = services.find(typeIndex);
        if (it == services.end())
        {
            throw std::runtime_error("Service not found");
        }
        return std::static_pointer_cast<T>(it->second);
    }
};