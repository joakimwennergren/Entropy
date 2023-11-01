#pragma once

#include <vector>
#include "context.hpp"
#include "vulkancontext.hpp"

using namespace Symbios::Core;
using namespace Symbios::Global;

namespace Symbios
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

                // Vulkan Ctx
                std::shared_ptr<Context> _context;

                // Semaphores
                std::vector<VkSemaphore> _imageSemaphores;
                std::vector<VkSemaphore> _renderFinishedSemaphores;

                // Fences
                std::vector<VkFence> _fences;
            };
        }
    }
}
