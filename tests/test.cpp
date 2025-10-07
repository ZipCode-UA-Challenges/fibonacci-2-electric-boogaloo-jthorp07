/**
 * @file test.cpp
 * 
 * @brief Test driver for the fibonacci and fibonacciRacer functions.
 * 
 * @details The test driver runs and times the fibonacci and fibonacciRacer
 *          functions for various inputs, and verifies the correctness of their
 *          outputs.
 */

#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>

#include "choose_timer_unit.hpp"
#include "fibonacci.hpp"
#include "uint256_t.hpp"
// Check if the user cheated by using the precomputed solutions
#ifdef PRECOMPUTE_FIBONACCI_HPP
#error "You are not allowed to use the precomputed Fibonacci solutions in your implementation!"
#endif
#include "precompute_fibonacci.hpp"

constexpr auto ONE_SECOND_IN_NANOSECONDS = std::chrono::nanoseconds(1'000'000'000);
constexpr int NUMBER_OF_RUNS = 10;
constexpr int RAN_VERY_FAST = -1;

void fibonacciVerifier(std::array<uint256_t, fibonacci::MAX_256_BIT_FIBONACCI_INDEX + 1>& results, int start, int end) {
    bool allGood = true;
    for (int i = start; i <= end; ++i) {
        if (results[i] != FIBONACCI_SOLUTIONS[i]) {
            std::cout << "Mismatch at index " << i << ": expected " << FIBONACCI_SOLUTIONS[i]
                      << ", got " << results[i] << std::endl;
            allGood = false;
        }
    }
    if (!allGood) {
        throw 1;
    }
    std::cout << "All Fibonacci numbers match!" << std::endl;

}

int main(int argc, char* argv[]) {

    std::array<uint256_t, fibonacci::MAX_256_BIT_FIBONACCI_INDEX + 1> results = {0};

    // Loop until we find the appropriate run to use.
    int finalFibonacciNumberCount = 0;
    int fibonacciNumbersToCompute = 1;
    while (true) {

        // Average the time over several runs for stability
        std::chrono::nanoseconds accumulator(0);
        for (int run = 0; run < NUMBER_OF_RUNS; ++run) {
            const auto start = std::chrono::high_resolution_clock::now();
            fibonacci::fibonacciRacer(results, 0, fibonacciNumbersToCompute);
            const auto end = std::chrono::high_resolution_clock::now();
            accumulator += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        }

        // If on average took over 1 second, reduce number computed by 1, then use that
        const auto averageDuration = accumulator / NUMBER_OF_RUNS;
        if (averageDuration >= ONE_SECOND_IN_NANOSECONDS) {
            finalFibonacciNumberCount = fibonacciNumbersToCompute - 1;
            break;
        }
        fibonacciNumbersToCompute += 1;
        if (fibonacciNumbersToCompute > fibonacci::MAX_256_BIT_FIBONACCI_INDEX) {
            finalFibonacciNumberCount = RAN_VERY_FAST;
            break;
        }


    }

    if (finalFibonacciNumberCount == RAN_VERY_FAST) {
        fibonacciVerifier(results, 0, fibonacci::MAX_256_BIT_FIBONACCI_INDEX);
    } else {
        fibonacciVerifier(results, 0, finalFibonacciNumberCount);
    }

    if (finalFibonacciNumberCount == RAN_VERY_FAST) {

        std::chrono::nanoseconds accumulator(0);
        for (int run = 0; run < NUMBER_OF_RUNS; ++run) {
            const auto start = std::chrono::high_resolution_clock::now();
            fibonacci::fibonacciRacer(results, 0, fibonacci::MAX_256_BIT_FIBONACCI_INDEX);
            const auto end = std::chrono::high_resolution_clock::now();
            accumulator += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        }
        const auto averageDuration = accumulator / NUMBER_OF_RUNS;
        const auto averageDurationReport = chooseTimerUnits(averageDuration);
        std::cout << "Your implementation computed all possible Fibonacci numbers for a" <<
        " 256-bit integer in " << averageDurationReport << '\n';
    } else {
        std::cout << "Your implementation computed Fibonacci numbers 0 to " << finalFibonacciNumberCount << " within 1 second.";
    }

    return 0;
}
