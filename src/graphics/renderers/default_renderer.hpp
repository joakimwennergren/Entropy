#pragma once

#include "context.hpp"
#include "commandbuffer.hpp"

using namespace Symbios::Core;
using namespace Symbios::Graphics;

namespace Symbios
{
    namespace Graphics
    {
        namespace Renderers
        {
            class Default
            {
            public:
                Default(Context *context, CommandBuffers::CommandBuffer commandBuffer);
                ~Default();

            private:
                Context* _context;
            };
        }

    }

}
