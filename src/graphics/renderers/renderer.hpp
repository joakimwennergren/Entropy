#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <flecs/flecs.h>

#include <graphics/renderpasses/renderpass.hpp>
#include <graphics/pipelines/skinned_pipeline.hpp>
#include <graphics/pipelines/cubemap_pipeline.hpp>
#include <graphics/pipelines/2d_pipeline.hpp>
#include <graphics/pipelines/gui_pipeline.hpp>
#include <graphics/pipelines/line_pipeline.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/commandbuffers/commandbuffer.hpp>
#include <graphics/data/ubo.hpp>
#include <graphics/buffers/uniformbuffer.hpp>
#include <graphics/buffers/indexbuffer.hpp>
#include <graphics/data/pushcontants.hpp>
#include <graphics/data/ubo.hpp>
#include <graphics/synchronization/synchronizer.hpp>
// #include <graphics/cubemaps/cubemap.hpp>
#include <servicelocators/servicelocator.hpp>
#include <graphics/descriptorsets/descriptorset.hpp>
#include <graphics/swapchains/swapchain.hpp>
#include <gltf/model.hpp>
#include <graphics/cameras/perspective_camera.hpp>
#include <graphics/primitives/3d/plane.hpp>

#include <graphics/primitives/2d/sprite.hpp>
#include <graphics/primitives/2d/quad.hpp>
#include <graphics/buffers/uniformbuffer.hpp>

#include <graphics/cameras/flying_camera.hpp>

#include <graphics/utilities/utilities.hpp>
#include <input/keyboard/keyboard.hpp>
#include <graphics/pipelines/gizmo_pipeline.hpp>

#include <graphics/synchronization/queuesync.hpp>

#include <tracy/Tracy.hpp>
#include <imgui.h>

#include <ecs/components/position.hpp>
#include <ecs/components/scale.hpp>
#include <ecs/components/model.hpp>
#include <ecs/components/renderable.hpp>
#include <ecs/components/gizmo.hpp>
#include <ecs/components/line.hpp>
#include <ecs/components/color.hpp>
#include <ecs/components/tags/scripted.hpp>

#include <timing/timer.hpp>

#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

using namespace Entropy::Graphics::Utilities;
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
using namespace Entropy::GLTF;
using namespace Entropy::Graphics::Synchronization;
// using namespace Entropy::Graphics::CubeMaps;
using namespace Entropy::Input;

namespace Entropy
{
    namespace Graphics
    {
        namespace Renderers
        {
            class Renderer
            {
            public:
                Renderer(std::shared_ptr<ServiceLocator> serviceLocator, flecs::world *world, float xscale, float yscale);
#ifdef BUILD_FOR_ANDROID
                Renderer(std::shared_ptr<ServiceLocator> serviceLocator, AAssetManager *assetManager);
                std::vector<char> loadShader(std::string filename, AAssetManager *assetManager);
#endif
                void Render(int width, int height, bool resize);
                VkResult DoRender(int width, int height);
                void DrawGizmo(Entropy::Components::Gizmo gizmo);
                void DrawGUI();
                VkResult SubmitAndPresent(VkCommandBuffer cmdBuffer, uint32_t imageIndex);
                void DrawEntity(flecs::entity entity, uint32_t index);
                void HandleResize(int width, int height);
                bool isResizing = true;
                struct UboDataDynamic
                {
                    glm::vec4 color;
                    glm::vec4 colorBorder;
                    glm::vec4 colorShadow;
                    glm::mat4 proj;
                    glm::mat4 view;
                    glm::mat4 invView;
                    glm::mat4 model;
                    glm::vec2 position;
                    glm::vec2 size;
                    glm::vec4 borderRadius;
                    int shapeId;
                    int time;
                };
                size_t dynamicAlignment{0};
                unsigned int _currentFrame = 0;
                uint32_t imageIndex;
                bool skip = false;
                std::shared_ptr<CommandBuffer> cmdBufferUI;
                std::shared_ptr<RenderPass> _renderPass;
                std::shared_ptr<Synchronizer> _synchronizer;
                std::vector<std::shared_ptr<CommandBuffer>> _commandBuffers;
                void Wireframe(bool on);
                std::shared_ptr<Camera> _camera;

            private:
                void Setup(std::shared_ptr<ServiceLocator> serviceLocator, float xscale, float yscale);

                std::unique_ptr<Pipeline> _pipeline;
                std::unordered_map<std::string, std::shared_ptr<Pipeline>> _pipelines;

                std::vector<VkBuffer> rawUniformBuffers;
                std::vector<VkBuffer> uniformBuffersInstances;

                std::vector<UniformBuffer *> _uniformBuffers;

                std::shared_ptr<ServiceLocator> _serviceLocator;

                std::shared_ptr<Descriptorset> _descriptorSet;
                std::shared_ptr<LogicalDevice> _logicalDevice;
                std::shared_ptr<Swapchain> _swapChain;

                VkCommandBuffer currentCmdBuffer;
                VkDescriptorSet currentDescriptorSet;

                /** @brief Properties of the physical device including limits that the application can check against */
                VkPhysicalDeviceProperties properties;

                // One big uniform buffer that contains all matrices
                // Note that we need to manually allocate the data to cope for GPU-specific uniform buffer offset alignments

                std::vector<UniformBuffer *> dynUbos;
                size_t bufferSize;

                VkResult imageResult;

                std::shared_ptr<QueueSync> _queueSync;

                size_t pad_uniform_buffer_size(size_t originalSize);

                uint32_t indexCount;
                uint32_t vertexCount;

                int32_t global_vtx_offset = 0;
                int32_t global_idx_offset = 0;

                std::shared_ptr<Keyboard> _keyboard;
                std::shared_ptr<Cam> _cam;
                std::unique_ptr<VertexBuffer> _vertexBuffer;
                std::unique_ptr<Buffer> _indexBuffer;
                Texture *fontTexture;
                flecs::world *_world;
                Timer *_timer;

                VkFence _fence;
            };
        }
    }
}
