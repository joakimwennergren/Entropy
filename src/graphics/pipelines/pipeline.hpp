#pragma once

#include <plog/Log.h>

#include "../shaders/shader.hpp"
#include "context.hpp"
#include "renderpass.hpp"
#include "filesystem.hpp"

using namespace Symbios::Filesystem;

namespace Symbios
{
    namespace Graphics
    {
        namespace Pipelines
        {
            class Pipeline
            {
            public:
                Pipeline(Symbios::Core::Context *context, Symbios::Graphics::RenderPasses::RenderPass *renderPass);
                ~Pipeline();
                VkPipeline pipeline;

            private:
                Symbios::Graphics::Shaders::Shader *_shader;
                Symbios::Core::Context *_context;
                VkPipelineLayout pipelineLayout;
            };
        }
    }
}
