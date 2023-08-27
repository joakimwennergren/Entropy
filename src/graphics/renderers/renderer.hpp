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
                void UpdateUniforms(uint32_t currentImage);
                std::shared_ptr<Context> _context;
                std::shared_ptr<RenderPass> _renderPass;
                std::vector<std::shared_ptr<CommandBuffer>> _commandBuffers;

                std::unique_ptr<Pipeline> _pipeline;

                std::vector<VkSemaphore> _imageAvailableSemaphores;
                std::vector<VkSemaphore> _renderFinishedSemaphores;
                std::vector<VkFence> _inFlightFences;

                uint32_t _currentFrame = 0;

                bool _framebufferResized = false;

                const std::vector<Vertex> vertices = {
                    {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
                    {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
                    {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
                    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

                const std::vector<uint16_t> indices = {
                    0, 1, 2, 2, 3, 0};

                // Buffers
                std::unique_ptr<Buffer> _vertexBuffer;
                std::unique_ptr<Buffer> _indexBuffer;

                std::vector<VkBuffer> uniformBuffers;
                std::vector<VkDeviceMemory> uniformBuffersMemory;
                std::vector<void *> uniformBuffersMapped;

                // textures
                std::unique_ptr<Texture> _texture;

                std::unique_ptr<Quad> _quad;

                std::vector<Quad *> _text;

                InstanceUBO _instanceUbo;
                std::vector<VkBuffer> uniformBuffersInstances;
                VkBuffer buf1, buf2;

                VkDeviceMemory _uniformBufferInstanceMemory;
                VkDeviceMemory _uniformBufferInstanceMemory2;
                void * mem1;
                void * mem2;
                uint32_t dynamicAlignment;

                // Text shaping
                unsigned int glyph_count;
                hb_glyph_info_t *glyph_info;
                hb_glyph_position_t *glyph_pos;
            };
        }
    }
}
