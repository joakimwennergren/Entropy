#pragma once

#include <iostream>

#include "context.hpp"
#include "quad.hpp"

using namespace Symbios::Core;
using namespace Symbios::Graphics::Primitives;

namespace Symbios
{
    namespace Graphics
    {
        namespace Factories
        {
            /**
             * @brief
             *
             */
            class PrimitiveFactory
            {
            public:
                /**
                 * @brief Construct a new Command Buffer object
                 *
                 * @param context
                 */

                PrimitiveFactory() = default;

                /**
                 * @brief Construct a new Command Buffer object
                 *
                 * @param context
                 */
                PrimitiveFactory(std::shared_ptr<Context> context);

                Quad * NewQuad();

                inline void Test() 
                {
                    std::cout << "TEST" << std::endl;
                }

            private:
                std::shared_ptr<Context> _context;
            };
        }
    }
}
