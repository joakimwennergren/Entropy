#pragma once

#include <math.h>

#ifndef PI
#define PI 3.1415926545
#endif

namespace Entropy
{
    namespace Animation
    {
        namespace EasingFunctions
        {
            double easeInSine(double t);
            double easeOutSine(double t);
            double easeInOutSine(double t);
            double easeInQuad(double t);
            double easeOutQuad(double t);
            double easeInOutQuad(double t);
            double easeInCubic(double t);
            double easeOutCubic(double t);
            double easeInOutCubic(double t);
            double easeInQuart(double t);
            double easeOutQuart(double t);
            double easeInOutQuart(double t);
            double easeInQuint(double t);
            double easeOutQuint(double t);
            double easeInOutQuint(double t);
            double easeInExpo(double t);
            double easeOutExpo(double t);
            double easeInOutExpo(double t);
            double easeInCirc(double t);
            double easeOutCirc(double t);
            double easeInOutCirc(double t);
            double easeInBack(double t);
            double easeOutBack(double t);
            double easeInOutBack(double t);
            double easeInElastic(double t);
            double easeOutElastic(double t);
            double easeInOutElastic(double t);
            double easeInBounce(double t);
            double easeOutBounce(double t);
            double easeInOutBounce(double t);
        }
    }
}