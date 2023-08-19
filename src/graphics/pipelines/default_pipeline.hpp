#pragma once

#include "../shaders/default_shader.hpp"
#include "context.hpp"

namespace Symbios
{
    namespace Graphics
    {
        namespace Pipeline
        {
            class Default
            {
            public:
                Default(std::shared_ptr<Symbios::Core::Context> context);

            private:
                std::unique_ptr<Symbios::Graphics::Shader::Default> _shader;
                std::shared_ptr<Symbios::Core::Context> _context;
            };
        }

    }

}
