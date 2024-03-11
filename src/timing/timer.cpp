#include "timer.hpp"

using namespace Entropy::Timing;

/**
 * @brief Constructor
 * @param tick_duration duration of each tick
 * @param max_tick max ticks
 */
Timer::Timer(float tick_duration, float const &max_tick)
{
    assert(tick_duration != 0.0f);

    SetTickDuration(tick_duration);
    SetMaxTick(max_tick);
    Reset();
}

/**
 * @brief Calculate
 * @return (void)
 */
void Timer::Calculate()
{
    _end = std::chrono::system_clock::now();
    _elapsed = _end - _start;
    float elapsedMilliseconds = _elapsed.count() * 1000.0f;

    if (elapsedMilliseconds >= _tick_duration && elapsedMilliseconds > 0.0f)
    {
        _update = true;
        float tickCount = elapsedMilliseconds / _tick_duration;
        _current_tick += tickCount;
        if (_current_tick >= _max_tick)
        {
            _current_tick = fmod(_current_tick, _max_tick);
        }
        _start = _end;
    }
    else
    {
        _update = false;
    }
}

/**
 * @brief return current tick
 * @return float tick
 */
float Timer::GetTick()
{
    Calculate();
    return _current_tick;
}

/**
 * @brief
 * @param d const float duration
 * @return (void)
 */
void Timer::SetTickDuration(float const &d)
{
    assert(d != 0.0f);
    _tick_duration = d;
}

/**
 * @brief Set max tick
 * @param max const float max tick
 * @return (void)
 */
void Timer::SetMaxTick(float const &max)
{
    assert(max != 0.0f);
    _max_tick = max;
}

/**
 * @brief Start the timer
 * @return (void)
 */
void Timer::Start()
{
    Reset();
}

/**
 * @brief Reset the timer
 * @return (void)
 */
void Timer::Reset()
{
    _update = true;
    _current_tick = 0.0f;
    _start = std::chrono::system_clock::now();
}

/**
 * @brief Check if timer is updated
 * @return bool
 */
bool Timer::Updated()
{
    return _update;
}