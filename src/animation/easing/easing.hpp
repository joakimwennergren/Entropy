/**
 * @file easing.hpp
 * @author Joakim Wennergren (joakim.wennergren@databeams.se)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <math.h>

namespace Symbios
{
    namespace Animation
    {
        namespace EasingFunctions
        {
            inline float easeInSine(float x)
            {
                return 1 - cos((x * M_PI) / 2);
            }
        }
    }
}