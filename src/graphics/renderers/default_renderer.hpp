#pragma once

#include "context.hpp"
#include "default_commandbuffer.hpp"

namespace Symbios
{
    namespace Graphics
    {
        namespace Renderers
        {
            class Default
            {
            public:
                Default(std::shared_ptr<Symbios::Core::Context> context, Symbios::Graphics::CommandBuffers::Default commandBuffer);
                ~Default();

            private:
                std::shared_ptr<Symbios::Core::Context> _context;
            };
        }

    }

}
