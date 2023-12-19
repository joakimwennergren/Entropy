#pragma once

#include <servicelocators/servicelocator.hpp>

using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Primitives;
using namespace Entropy::SceneGraphs;
using namespace Entropy::Physics;

namespace Entropy
{
    namespace Events
    {
        namespace Queues
        {
            class RenderQueue
            {
            public:
                RenderQueue(std::shared_ptr<ServiceLocator> serviceLocator){

                };

            private:
                eventpp::EventQueue<unsigned int, void(const std::string &, const bool)> _queue;
                unsigned int _index;
            };
        }
    }
}
