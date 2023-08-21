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
        namespace Pipeline
        {
            class Default
            {
            public:
                Default(Symbios::Core::Context *context, Symbios::Graphics::RenderPasses::Default *renderPass);
                ~Default();
                VkPipeline pipeline;

            private:
                Symbios::Graphics::Shaders::Shader *_shader;
                Symbios::Core::Context *_context;
                VkPipelineLayout pipelineLayout;
            };
        }
    }
}
