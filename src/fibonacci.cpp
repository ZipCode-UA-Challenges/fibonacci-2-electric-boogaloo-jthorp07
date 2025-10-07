/**
 * @file fibonacci.cpp
 * 
 * @brief Implementation file for the fibonacci and fibonacciRacer
 *        free functions declared in include/fibonacci.hpp.
 */

#include "fibonacci.hpp"
#include "uint256_t.hpp"

// Used in Binet's Formula Solution
#include <cmath>

namespace {

// Matrix Exponentiation Definitions
typedef std::array<std::array<uint256_t, 2>, 2> Matrix2x2;
constexpr Matrix2x2 makeFibMatrix() {
    return { uint256_t(1), uint256_t(1), uint256_t(1), uint256_t(0) };
}
constexpr Matrix2x2 FIB_M = makeFibMatrix();
void inline fibMultiply(Matrix2x2& fibMatrix, const Matrix2x2& other) {
    uint256_t a = fibMatrix[0][0] * other[0][0] + fibMatrix[0][1] * other[1][0];
    uint256_t b = fibMatrix[0][0] * other[0][1] + fibMatrix[0][1] * other[1][1];
    uint256_t c = fibMatrix[1][0] * other[0][0] + fibMatrix[1][1] * other[1][0];
    uint256_t d = fibMatrix[1][0] * other[0][1] + fibMatrix[1][1] * other[1][1];

    fibMatrix[0][0] = a;
    fibMatrix[0][1] = b;
    fibMatrix[1][0] = c;
    fibMatrix[1][1] = d;
}
void inline fibMultiply(Matrix2x2& fibMatrix) {
    fibMultiply(fibMatrix, FIB_M);
}
void fibPower(Matrix2x2& fibMatrix, uint32_t n) {
    if (n == 0 || n == 1) return;
    fibPower(fibMatrix, n / 2);
    fibMultiply(fibMatrix, fibMatrix);
    if (n % 2 != 0) fibMultiply(fibMatrix);
}

} // anonymous namespace

namespace fibonacci {

void fibonacciRacer(std::array<uint256_t, MAX_256_BIT_FIBONACCI_INDEX + 1>& results, int start, int end) {
    for (int i = start; i <= end; i++) {
        results[i] = fibonacci(i);
    }
}


uint256_t fibonacci(int n) {

    // // Naive Recursive Solution: Computed fibonacci numbers 0 to 38 within allotted time
    // if (n <= 1) return uint256_t(n);
    // return fibonacci(n - 1) + fibonacci(n - 2);

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

    // // Closed Form Solution (Binet's Formula): Fails due to precision errors after fibonacci(91), and overflow after fibonacci(93)
    // static const long double sqrt5 = sqrtl(5.0L);
    // static const long double invSqrt5 = 1.0L / sqrt5;
    // const long double termOne = powl((1 + sqrt5) / 2.0L, n);
    // const long double termTwo = powl((1 - sqrt5) / 2.0L, n);
    // const long double result = invSqrt5 * (termOne - termTwo);
    // return uint256_t(result);

    // Matrix Exponentiation Solution
    if (n == 0) return uint256_t(0);
    auto fibMatrix = makeFibMatrix();
    fibPower(fibMatrix, n - 1);
    return fibMatrix[0][0];

}

} // namespace fibonacci
