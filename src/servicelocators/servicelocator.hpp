#pragma once

#include <map>
#include <memory>
#include <typeindex>
#include <services/service.hpp>

using namespace Entropy::Services;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpotentially-evaluated-expression"

namespace Entropy
{
    namespace ServiceLocators
    {
        class ServiceLocator
        {
        public:
            /**
             * @brief Add a service
             * @param service T service
             * @return (void)
             */
            void AddService(std::shared_ptr<Service> service)
            {
                services[std::type_index(typeid(*service))] = service;
            }

            /**
             * @brief Get a service
             * @return T service
             */
            template <typename T>
            std::shared_ptr<T> GetService()
            {

                std::type_index index(typeid(T));
                if (services.count(std::type_index(typeid(T))) != 0)
                {
                    return std::static_pointer_cast<T>(services[index]);
                }
                else
                {
                    return nullptr;
                }
            }

        private:
            std::map<std::type_index, std::shared_ptr<Service>> services;
        };
    }
}

#pragma GCC diagnostic pop