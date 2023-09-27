#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <chrono>

#include <unordered_map>

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
#include "scenegraph.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <freetype/ftglyph.h>
#include <iostream>

#include "renderable.hpp"
#include "sprite.hpp"

#include <map>

using namespace Symbios::Core;
using namespace Symbios::Renderables;
using namespace Symbios::Graphics::Buffers;
using namespace Symbios::Graphics::Textures;
using namespace Symbios::Graphics::Pipelines;
using namespace Symbios::Graphics::Primitives;
using namespace Symbios::Graphics::RenderPasses;
using namespace Symbios::Graphics::CommandBuffers;

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
                 * @param context
                 */
                Renderer();

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

                std::vector<VkBuffer> rawUniformBuffers;

                InstanceUBO _instanceUbo;
                std::vector<VkBuffer> uniformBuffersInstances;

                VkDeviceMemory _uniformBufferInstanceMemory;
                VkDeviceMemory _uniformBufferInstanceMemory2;
                VkDeviceSize dynamicAlignment;

                // @refactored buffers!!
                std::vector<UniformBuffer *> _uniformBuffers;

                // @todo remove this
                Sprite *ivy7; 
            };
        }
    }
}
