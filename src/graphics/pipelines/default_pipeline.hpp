#pragma once

#include "../shaders/default_shader.hpp"

namespace Symbios
{
    namespace Graphics
    {
        namespace Pipeline
        {
            class Default
            {
            public:
                Default() = default;

            private:
                std::unique_ptr<Symbios::Graphics::Shader::Default> _shader;
            };
        }

    }

}
