/**
 * @file pipeline.hpp
 * @author Joakim Wennergren (joakim.wennergren@databeams.se)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "shader.hpp"
#include "context.hpp"
#include "renderpass.hpp"
#include "filesystem.hpp"

using namespace Symbios::Core;
using namespace Symbios::Filesystem;
using namespace Symbios::Graphics::Shaders;
using namespace Symbios::Graphics::RenderPasses;

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
                Pipeline(std::shared_ptr<Context> context, std::shared_ptr<RenderPass> renderPass);

                /**
                 * @brief Destroy the Pipeline object
                 *
                 */
                ~Pipeline();

                /**
                 * @brief Get the Pipeline object
                 *
                 * @return VkPipeline
                 */
                inline VkPipeline GetPipeline() { return this->_pipeline; };

            private:
                std::shared_ptr<Context> _context;
                std::unique_ptr<Shader> _shader;
                VkPipelineLayout _pipelineLayout;
                VkPipeline _pipeline;
            };
        }
    }
}
