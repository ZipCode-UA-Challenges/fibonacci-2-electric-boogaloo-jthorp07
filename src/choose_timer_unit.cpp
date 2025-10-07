/**
 * @file choose_timer_unit.cpp
 * 
 * @brief Implementation file for the chooseTimerUnits function.
 */

#include "choose_timer_unit.hpp"
#include <chrono>
#include <string>

std::string chooseTimerUnits(std::chrono::nanoseconds durationNanoseconds) {
    using namespace std::chrono;

    if (durationNanoseconds.count() < 1000) {
        return std::to_string(durationNanoseconds.count()) + " Nanoseconds";
    } else if (durationNanoseconds.count() < 1'000'000) {
        auto microsecondsDuration = duration_cast<microseconds>(durationNanoseconds);
        return std::to_string(microsecondsDuration.count()) + " Microseconds";
    } else if (durationNanoseconds.count() < 1'000'000'000) {
        auto millisecondsDuration = duration_cast<milliseconds>(durationNanoseconds);
        return std::to_string(millisecondsDuration.count()) + " Milliseconds";
    } else {
        auto secondsDuration = duration_cast<seconds>(durationNanoseconds);
        return std::to_string(secondsDuration.count()) + " Seconds";
    }
}
