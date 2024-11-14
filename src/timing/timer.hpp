#pragma once

#include <cassert>
#include <chrono>

namespace Entropy::Timing {
 /**
  * A simple class for measuring time durations.
  */
 class Timer {
 public:
  /**
   * Constructs a Timer object with the specified tick duration and maximum tick
   * value.
   *
   * @param tick_duration The duration of each tick in seconds.
   * @param max_tick The maximum tick value (default is 10000.0f).
   *
   * @returns None
   */
  explicit Timer(float tick_duration, float const &max_tick = 10000.0f);

  /**
   * Gets the current tick value.
   *
   * @returns The current tick value as a float.
   */
  float GetTick();

  /**
   * Set the duration of a tick.
   *
   * @param d The duration of a tick to be set.
   *
   * @returns None
   */
  void SetTickDuration(float const &d);

  /**
   * Set the maximum tick value.
   *
   * @param max The maximum tick value to be set.
   *
   * @returns None
   */
  void SetMaxTick(float const &max);

  /**
   * Starts the process or operation.
   *
   * @returns None
   */
  void Start();

  /**
   * Resets the state of an object to its initial state.
   *
   * @returns None
   */
  void Reset();

  /**
   * Checks if an update has occurred.
   *
   * @returns True if an update has occurred, false otherwise.
   */
  bool Updated();

 private:
  /**
   * Calculates a result based on the internal state of the object.
   *
   * @returns None
   */
  void Calculate();

  float _tick_duration{};
  float _max_tick{};
  float _current_tick{};
  bool _update{};
  std::chrono::time_point<std::chrono::system_clock> _start;
  std::chrono::time_point<std::chrono::system_clock> _end;
  std::chrono::duration<float> _elapsed{};
 };
} // namespace Entropy::Timing

