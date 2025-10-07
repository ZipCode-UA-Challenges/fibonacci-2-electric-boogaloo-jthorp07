/**
 * @file fibonacci.cpp
 * 
 * @brief Implementation file for the fibonacci and fibonacciRacer
 *        free functions declared in include/fibonacci.hpp.
 */

#include "fibonacci.hpp"
#include "uint256_t.hpp"

namespace fibonacci {

void fibonacciRacer(std::array<uint256_t, MAX_256_BIT_FIBONACCI_INDEX + 1>& results, int start, int end) {
    for (int i = start; i <= end; i++) {
        results[i] = fibonacci(i);
    }
}

uint256_t fibonacci(int n) {

    // Naive Recursive Solution: 
    if (n <= 1) return uint256_t(n);
    return fibonacci(n - 1) + fibonacci(n - 2);

    // // Memoization Solution: Ran in 689 Nanoseconds
    // static std::array<uint256_t, MAX_256_BIT_FIBONACCI_INDEX + 1> cache{0, 1};
    // static int highestComputed = 1;
    // if (n > highestComputed) {
    //     for (int i = highestComputed + 1; i <= n; i++) {
    //         cache[i] = cache[i - 1] + cache[i - 2];
    //     }
    //     highestComputed = n;
    // }
    // return cache[n];
}

} // namespace fibonacci
