#pragma once

#include <chrono>
#include <cmath>
#include <cassert>

namespace Entropy
{
    namespace Timing
    {
        class Timer
        {
        public:
            Timer(float tick_duration, float const &max_tick = 10000.0f);
            float GetTick();
            void SetTickDuration(float const &d);
            void SetMaxTick(float const &max);
            void Start();
            void Reset();
            bool Updated();

        private:
            float _tick_duration;
            float _max_tick;
            float _current_tick;
            bool _update;
            std::chrono::time_point<std::chrono::system_clock> _start;
            std::chrono::time_point<std::chrono::system_clock> _end;
            std::chrono::duration<float> _elapsed;
            void Calculate();
        };
    }
}