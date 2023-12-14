/*
 * This timer handles ticks, ticks are a predetermined duration, usually in msecs.
 * This is part of 2f, a 2D game engine based on SFML I'm currently creating.
 */

#pragma once

#include <chrono>
#include <cmath>

namespace Entropy
{
    namespace Timing
    {
        class Timer
        {
        protected:
            float _tick_duration; // Tick duration in msec
            float _max_tick;      // Max ticks before max ticks gets back to 0
            float _current_tick;  // Current tick
            bool _update;         // If the tick count has updated
            /* Timers */
            std::chrono::time_point<std::chrono::system_clock> _start; // Time start
            std::chrono::time_point<std::chrono::system_clock> _end;   // End
            std::chrono::duration<float> _elapsed;                     // Delta
            /* Calculations */
            void calculate(); // Calculates the stuff
        public:
            Timer();                                                      // Default constructor
            Timer(float tick_duration, float const &max_tick = 10000.0f); // Creates a timer
            /* Getters */
            float get_tick(); // Returns current tick
            /* Setters */
            void set_tick_duration(float const &d); // Sets tick duration
            void set_max_tick(float const &max);    // Sets max tick
            /* Methods */
            void start();   // Starts the timer
            void reset();   // Resets the timer
            bool updated(); // If the tick count has changed
        };
    }
}