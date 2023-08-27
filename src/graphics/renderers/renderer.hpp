#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <chrono>

#include "context.hpp"
#include "renderpass.hpp"
#include "pipeline.hpp"
#include "vertex.hpp"
#include "buffer.hpp"
#include "commandbuffer.hpp"
#include "mvpuniform.hpp"
#include "texture.hpp"
#include "filesystem.hpp"
#include "quad.hpp"
#include "instance.hpp"
#include "instance_pushcontants.hpp"

#include "uniformbuffer.hpp"

#include <hb.h>

using namespace Symbios::Core;
using namespace Symbios::Graphics::Buffers;
using namespace Symbios::Graphics::Textures;
using namespace Symbios::Graphics::Pipelines;
using namespace Symbios::Graphics::RenderPasses;
using namespace Symbios::Graphics::CommandBuffers;
using namespace Symbios::Graphics::Primitives;

namespace Symbios
{
    namespace Graphics
    {
        namespace Renderers
        {
            class Renderer
            {
            public:
                /**
                 * @brief Construct a new Renderer object
                 *
                 */
                Renderer() = default;

                /**
                 * @brief Construct a new Renderer object
                 *
                 * @param context
                 */
                Renderer(std::shared_ptr<Context> context);

                /**
                 * @brief
                 *
                 */
                void Render();

                /**
                 * @brief
                 *
                 */
                ~Renderer();

                /**
                 * @brief
                 *
                 */
                void FrameBufferResized() { this->_framebufferResized = true; };

            private:
                std::shared_ptr<Context> _context;
                std::shared_ptr<RenderPass> _renderPass;
                std::vector<std::shared_ptr<CommandBuffer>> _commandBuffers;

                std::unique_ptr<Pipeline> _pipeline;

                std::vector<VkSemaphore> _imageAvailableSemaphores;
                std::vector<VkSemaphore> _renderFinishedSemaphores;
                std::vector<VkFence> _inFlightFences;

                uint32_t _currentFrame = 0;

                bool _framebufferResized = false;

                std::vector<Quad *> _sprites;
                std::vector<VkBuffer> rawUniformBuffers;

                InstanceUBO _instanceUbo;
                std::vector<VkBuffer> uniformBuffersInstances;
                VkBuffer buf1, buf2;

                VkDeviceMemory _uniformBufferInstanceMemory;
                VkDeviceMemory _uniformBufferInstanceMemory2;
                void *mem1;
                void *mem2;
                uint32_t dynamicAlignment;

                // Text shaping
                unsigned int glyph_count;
                hb_glyph_info_t *glyph_info;
                hb_glyph_position_t *glyph_pos;

                // @refactored buffers!!
                std::vector<UniformBuffer *> _uniformBuffers;
            };
        }
    }
}
