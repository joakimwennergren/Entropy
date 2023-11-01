#pragma once

#include "shader.hpp"
#include "context.hpp"
#include "renderpass.hpp"
#include "filesystem.hpp"
#include "vertex.hpp"
#include "instance.hpp"
#include "instance_pushcontants.hpp"
#include "vulkancontext.hpp"

using namespace Symbios::Core;
using namespace Symbios::Filesystem;
using namespace Symbios::Graphics::Shaders;
using namespace Symbios::Graphics::RenderPasses;
using namespace Symbios::Global;

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
                 */
                Pipeline() = default;

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

                /**
                 * @brief
                 *
                 */
                void Build();

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
