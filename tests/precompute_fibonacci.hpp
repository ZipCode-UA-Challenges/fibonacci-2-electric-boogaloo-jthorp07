/**
 * @file precompute_fibonacci.hpp
 * 
 * @brief Include file for a compile-time precomputed Fibonacci sequence.
 * 
 * @details This file declares a `constexpr` array containing the first 94
 *          Fibonacci numbers, which fit within a 64-bit unsigned integer.
 *          This can be used for testing and verification purposes.
 */

#ifndef PRECOMPUTE_FIBONACCI_HPP
#define PRECOMPUTE_FIBONACCI_HPP

#include "uint256_t.hpp"
#include <array>
#include <cstdint>

constexpr int SOLUTION_COUNT = 94; // Number of Fibonacci numbers to precompute
constexpr int SOLUTION_COUNT_256BIT = 375; // Number of Fibonacci numbers to precompute for 256-bit

constexpr std::array<uint256_t, SOLUTION_COUNT_256BIT> precomputeFiconacci() {
    std::array<uint256_t, SOLUTION_COUNT_256BIT> fibs = {0};
    fibs[0] = 0;
    fibs[1] = 1;
    for (int i = 2; i < SOLUTION_COUNT_256BIT; ++i) {
        fibs[i] = fibs[i - 1] + fibs[i - 2];
    }
    return fibs;
}

constexpr std::array<uint256_t, SOLUTION_COUNT_256BIT> FIBONACCI_SOLUTIONS = precomputeFiconacci();

#endif // PRECOMPUTE_FIBONACCI_HPP
