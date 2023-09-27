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
            float InOutQuadBlend(float t)
            {
                if(t <= 0.5f)
                    return 2.0f * t * t;
                t -= 0.5f;
                return 2.0f * t * (1.0f - t) + 0.5f;
            }
        }
    }
}