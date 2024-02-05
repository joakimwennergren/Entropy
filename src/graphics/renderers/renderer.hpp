#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <graphics/renderpasses/renderpass.hpp>
#include <graphics/pipelines/pipeline.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/commandbuffers/commandbuffer.hpp>
#include <graphics/data/ubo.hpp>
#include <graphics/buffers/uniformbuffer.hpp>
#include <graphics/data/pushcontant.hpp>
#include <graphics/data/ubo.hpp>
#include <graphics/synchronization/synchronizer.hpp>
#include <renderables/renderable.hpp>
#include <scenegraphs/scenegraph.hpp>

#include <servicelocators/servicelocator.hpp>
#include <graphics/descriptorsets/descriptorset.hpp>
#include <graphics/swapchains/swapchain.hpp>
#include <gltf/model.hpp>
#include <graphics/cameras/perspective_camera.hpp>

#include <graphics/primitives/2d/sprite.hpp>
#include <graphics/buffers/uniformbuffer.hpp>

#include <graphics/utilities/utilities.hpp>

using namespace Entropy::SceneGraphs;
using namespace Entropy::Graphics::Utilities;
using namespace Entropy::Renderables;
using namespace Entropy::Graphics::Buffers;
using namespace Entropy::Graphics::Textures;
using namespace Entropy::Graphics::Pipelines;
using namespace Entropy::Graphics::RenderPasses;
using namespace Entropy::Graphics::CommandBuffers;
using namespace Entropy::Graphics::Synchronization;
using namespace Entropy::Graphics::Descriptorsets;
using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Swapchains;
using namespace Entropy::Graphics::Primitives;

namespace Entropy
{
    namespace Graphics
    {
        namespace Renderers
        {
            class Renderer
            {
            public:
                Renderer(std::shared_ptr<ServiceLocator> serviceLocator);
                Renderer(uint32_t *vertContent, uint32_t vertSize, uint32_t *fragContent, uint32_t fragSize);

                void Render(int width, int height);
                void SubmitAndPresent(VkCommandBuffer cmdBuffer, uint32_t imageIndex);
                void DrawRenderable(std::shared_ptr<Renderable> renderable, int width, int height, uint32_t modelIndex);

            private:
                std::shared_ptr<RenderPass> _renderPass;
                std::vector<std::shared_ptr<CommandBuffer>> _commandBuffers;

                std::unique_ptr<Pipeline> _pipeline;

                std::vector<VkBuffer> rawUniformBuffers;
                std::vector<VkBuffer> uniformBuffersInstances;

                std::vector<UniformBuffer *> _uniformBuffers;
                std::unique_ptr<Synchronizer> _synchronizer;

                std::shared_ptr<ServiceLocator> _serviceLocator;

                unsigned int _currentFrame = 0;

                std::shared_ptr<Descriptorset> _descriptorSet;
                std::shared_ptr<LogicalDevice> _logicalDevice;
                std::shared_ptr<Swapchain> _swapChain;

                std::shared_ptr<SceneGraph> _sceneGraph;
                std::shared_ptr<Camera> _camera;

                VkCommandBuffer currentCmdBuffer;
                VkDescriptorSet currentDescriptorSet;

                /** @brief Properties of the physical device including limits that the application can check against */
                VkPhysicalDeviceProperties properties;

                // One big uniform buffer that contains all matrices
                // Note that we need to manually allocate the data to cope for GPU-specific uniform buffer offset alignments
                struct UboDataDynamic
                {
                    glm::vec4 color;
                    glm::mat4 model;
                };

                std::vector<UniformBuffer *> dynUbos;
                size_t bufferSize;

                size_t dynamicAlignment{0};

                size_t pad_uniform_buffer_size(size_t originalSize);
            };
        }
    }
}
