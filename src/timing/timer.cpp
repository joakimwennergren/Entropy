#include "timer.hpp"

using namespace Entropy::Timing;

Timer::Timer(float tick_duration, float const &max_tick) {
  assert(tick_duration != 0.0f);

  SetTickDuration(tick_duration);
  SetMaxTick(max_tick);
  Reset();
}

void Timer::Calculate() {
  _end = std::chrono::system_clock::now();
  _elapsed = _end - _start;
  float elapsedMilliseconds = _elapsed.count() * 1000.0f;

  if (elapsedMilliseconds >= _tick_duration && elapsedMilliseconds > 0.0f) {
    _update = true;
    float tickCount = elapsedMilliseconds / _tick_duration;
    _current_tick += tickCount;
    if (_current_tick >= _max_tick) {
      _current_tick = fmod(_current_tick, _max_tick);
    }
    _start = _end;
  } else {
    _update = false;
  }
}

float Timer::GetTick() {
  Calculate();
  return _current_tick;
}

void Timer::SetTickDuration(float const &d) {
  assert(d != 0.0f);
  _tick_duration = d;
}

void Timer::SetMaxTick(float const &max) {
  assert(max != 0.0f);
  _max_tick = max;
}

void Timer::Start() { Reset(); }

void Timer::Reset() {
  _update = true;
  _current_tick = 0.0f;
  _start = std::chrono::system_clock::now();
}

bool Timer::Updated() { return _update; }
