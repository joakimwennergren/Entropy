#pragma once

#include <plog/Log.h>

#include "../shaders/default_shader.hpp"
#include "context.hpp"
#include "renderpass.hpp"



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
                Symbios::Graphics::Shader::Default *_shader;
                Symbios::Core::Context *_context;
                VkPipelineLayout pipelineLayout;
            };
        }

    }

}
