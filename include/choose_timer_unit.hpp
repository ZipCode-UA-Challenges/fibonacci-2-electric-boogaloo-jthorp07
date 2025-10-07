/**
 * @file choose_timer_unit.hpp
 * 
 * @brief Include file for the chooseTimerUnits function.
 */

#ifndef CHOOSE_TIMER_UNIT_HPP
#define CHOOSE_TIMER_UNIT_HPP

#include <chrono>
#include <string>

/**
 * @brief Chooses an appropriate time unit (nanoseconds, microseconds, milliseconds, or seconds)
 *        based on the given duration in nanoseconds, and returns a string representation of the duration
 *        in the chosen unit.
 * 
 * @param[in] durationNanoseconds The duration in nanoseconds.
 * 
 * @return A string representing the duration in the most suitable time unit.
 */
std::string chooseTimerUnits(std::chrono::nanoseconds durationNanoseconds);

#endif // CHOOSE_TIMER_UNIT_HPP
