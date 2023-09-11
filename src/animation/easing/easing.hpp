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

            inline float easeOutBounce(float x) {
                const float n1 = 7.5625;
                const float d1 = 2.75;
                if (x < 1 / d1) {
                return n1 * x * x;
                } else if (x < 2 / d1) {
                    return n1 * (x -= 1.5 / d1) * x + 0.75;
                } else if (x < 2.5 / d1) {
                    return n1 * (x -= 2.25 / d1) * x + 0.9375;
                } else {
                    return n1 * (x -= 2.625 / d1) * x + 0.984375;
                }
            }

            inline float easeInBounce(float x) {
                return 1 - easeOutBounce(1 - x);
            }

            inline float easeInOutBounce(float x) {
                return x < 0.5
                ? (1 - easeOutBounce(1 - 2 * x)) / 2
                : (1 + easeOutBounce(2 * x - 1)) / 2;
            }
        }
    }
}