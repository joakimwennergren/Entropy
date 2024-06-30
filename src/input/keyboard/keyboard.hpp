#pragma once
#include <config.hpp>
#include "spdlog/spdlog.h"


namespace Entropy
{
    namespace Input
    {
        class Keyboard
        {
        public:
            Keyboard();
            bool _w;
            bool _a;
            bool _s;
            bool _d;

        private:
        };
    }
}