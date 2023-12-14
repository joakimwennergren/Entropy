#pragma once

#include <vector>
#include <global/vulkancontext.hpp>

using namespace Entropy::Global;

namespace Entropy
{
    namespace Graphics
    {
        namespace Synchronization
        {
            class Synchronizer
            {
            public:
                /**
                 * @brief Construct a new Renderer object
                 *
                 * @param context
                 */
                Synchronizer(unsigned int numObjects);

                /**
                 * @brief
                 *
                 */
                ~Synchronizer();

                inline std::vector<VkSemaphore> GetImageSemaphores() { return this->_imageSemaphores; };
                inline std::vector<VkSemaphore> GetRenderFinishedSemaphores() { return this->_renderFinishedSemaphores; };
                inline std::vector<VkFence> GetFences() { return this->_fences; };

            private:
                unsigned int numObjects;

                // Semaphores
                std::vector<VkSemaphore> _imageSemaphores;
                std::vector<VkSemaphore> _renderFinishedSemaphores;

                // Fences
                std::vector<VkFence> _fences;
            };
        }
    }
}
