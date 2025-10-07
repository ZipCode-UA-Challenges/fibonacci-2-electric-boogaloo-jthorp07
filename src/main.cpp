#include <chrono>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

#include "choose_timer_unit.hpp"
#include "fibonacci.hpp"
#include "uint256_t.hpp"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "An integer argument is required to run this program!\n"
                  << "Example: \"" << argv[0] << " 100\"\n";
        return 1;
    }

    int n;
    try {
        n = std::stoi(argv[1]);
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Error: Invalid argument. Not a number: " << argv[1] << "\n";
        return 1;
    } catch (const std::out_of_range& oor) {
        std::cerr << "Error: Argument out of range: " << argv[1] << "\n";
        return 1;
    }

    if (n < 0 || n > fibonacci::MAX_256_BIT_FIBONACCI_INDEX) {
        std::cerr << "Error: Argument must be between 0 and " << fibonacci::MAX_256_BIT_FIBONACCI_INDEX << ".\n";
        return 1;
    }

    const auto startSingle = std::chrono::high_resolution_clock::now();
    // Store result in a volatile variable to strongly suggest no compiler optimizations
    uint256_t result = fibonacci::fibonacci(n);
    volatile uint256_t sink = result; // Tell the compiler not to optimize away the call
    const auto endSingle = std::chrono::high_resolution_clock::now();
    const auto durationNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(endSingle - startSingle);

    std::array<uint256_t, fibonacci::MAX_256_BIT_FIBONACCI_INDEX + 1> results = {UINT64_C(0)};
    const auto startRacer = std::chrono::high_resolution_clock::now();
    fibonacci::fibonacciRacer(results, 0, n);
    const auto endRacer = std::chrono::high_resolution_clock::now();
    const auto durationRacerNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(endRacer - startRacer);
    const std::string durationReport = chooseTimerUnits(durationNanoseconds);
    const std::string durationRacerReport = chooseTimerUnits(durationRacerNanoseconds);

    std::cout << "fibonacci::fibonacci(" << n << ") = " << result << '\n';
    std::cout << "Computed fibonacci::fibonacci(" << n << ") in " << durationReport << "\n";
    std::cout << "Computed fibonacci::fibonacciRacer(0, " << n << ") in " << durationRacerReport << "\n";
    return 0;
}