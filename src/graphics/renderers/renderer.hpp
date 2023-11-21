#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <chrono>

#include <unordered_map>

#include <contexts/context.hpp>
#include <graphics/renderpasses/renderpass.hpp>
#include <graphics/pipelines/pipeline.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/buffers/buffer.hpp>
#include <graphics/commandbuffers/commandbuffer.hpp>
#include <graphics/data/ubo.hpp>
#include <graphics/buffers/uniformbuffer.hpp>
#include <graphics/textures/texture.hpp>
#include <filesystem/filesystem.hpp>
#include <graphics/primitives/2d/quad.hpp>
#include <graphics/data/pushcontant.hpp>

#include <graphics/data/ubo.hpp>
#include <global/scenegraph.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <freetype/ftglyph.h>
#include <iostream>

#include <renderables/renderable.hpp>
#include <graphics/primitives/2d/sprite.hpp>

#include <graphics/synchronization/synchronizer.hpp>

#include <map>

using namespace Symbios::Core;
using namespace Symbios::Renderables;
using namespace Symbios::Graphics::Buffers;
using namespace Symbios::Graphics::Textures;
using namespace Symbios::Graphics::Pipelines;
using namespace Symbios::Graphics::Primitives;
using namespace Symbios::Graphics::RenderPasses;
using namespace Symbios::Graphics::CommandBuffers;
using namespace Symbios::Graphics::Synchronization;

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
                void FrameBufferResized() { this->_framebufferResized = true; };

            private:
                // Vulkan Ctx
                std::shared_ptr<Context> _context;
                std::shared_ptr<RenderPass> _renderPass;
                std::vector<std::shared_ptr<CommandBuffer>> _commandBuffers;

                std::unique_ptr<Pipeline> _pipeline;

                std::vector<VkSemaphore> _imageAvailableSemaphores;
                std::vector<VkSemaphore> _renderFinishedSemaphores;
                std::vector<VkFence> _inFlightFences;

                unsigned int _currentFrame = 0;

                bool _framebufferResized = false;

                std::vector<VkBuffer> rawUniformBuffers;
                std::vector<VkBuffer> uniformBuffersInstances;

                VkDeviceSize dynamicAlignment;

                std::vector<UniformBuffer *> _uniformBuffers;
                std::unique_ptr<Synchronizer> _synchronizer;
            };
        }
    }
}
