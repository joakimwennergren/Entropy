#pragma once

#include <graphics/shaders/shader.hpp>
#include <contexts/context.hpp>
#include <graphics/renderpasses/renderpass.hpp>
#include <filesystem/filesystem.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/data/pushcontant.hpp>
#include <global/vulkancontext.hpp>

using namespace Symbios::Core;
using namespace Symbios::Filesystem;
using namespace Symbios::Graphics::Shaders;
using namespace Symbios::Graphics::RenderPasses;
using namespace Entropy::Global;

namespace Symbios
{
    namespace Graphics
    {
        namespace Pipelines
        {
            /**
             * @brief
             *
             */
            class Pipeline
            {
            public:
                /**
                 * @brief Construct a new Pipeline object
                 *
                 * @param context
                 * @param renderPass
                 */
                Pipeline(std::shared_ptr<RenderPass> renderPass);

                /**
                 * @brief Destroy the Pipeline object
                 *
                 */
                ~Pipeline();

                inline VkPipeline GetPipeline() { return this->_pipeline; };
                inline VkPipelineLayout GetPipelineLayout() { return this->_pipelineLayout; };

            private:
                std::shared_ptr<Context> _context;
                std::unique_ptr<Shader> _shader;
                std::shared_ptr<RenderPass> _renderPass;

                VkPipelineLayout _pipelineLayout;
                VkPipeline _pipeline;
                VkDescriptorSetLayout _descriptorSetLayout;
            };
        }
    }
}
