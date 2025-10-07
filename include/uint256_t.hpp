/**
 * @file uint256_t.hpp
 * 
 * @brief Include file for uint256_t class.
 * 
 * @note This implementation was assisted by AI (Gemini Pro). While I manually wrote and checked
 *       the code, there may still be errors.
 */

#ifndef UINT256_T_HPP
#define UINT256_T_HPP

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>

// If the compiler supports __uint128_t, use it for performance
#if defined(__GNUC__) || defined(__clang__)
#define SUPPORTS_UINT128_EXTENSION
#endif

// If using MSVC, define div10_u64_no128
#if defined(_MSC_VER)
#include <intrin.h>
#endif

// Portable long division by 10 (used for insertion to ostream)
constexpr uint64_t TEN_64_BIT = 10ULL;
constexpr uint64_t ONE_64_BIT = 1ULL;
static inline uint64_t div10_u64_no128(uint64_t part, uint32_t& remainder) {
#if defined(_MSC_VER) && defined(_M_X64)
    uint64_t tmpRemainder = remainder;
    uint64_t quotient = _udiv128(tmpRemainder, part, TEN_64_BIT, &tmpRemainder);
    remainder = static_cast<uint32_t>(tmpRemainder);
    return quotient;
#else
    uint64_t quotient = 0;
    uint64_t tmpRemainder = remainder;
    for (int bit = 63; bit >= 0; --bit) {
        tmpRemainder = (tmpRemainder << 1) | ((part >> bit) & ONE_64_BIT);
        if (tmpRemainder >= TEN_64_BIT) {
            tmpRemainder -= TEN_64_BIT;
            quotient |= (ONE_64_BIT << bit);
        }
    }
    remainder = static_cast<uint32_t>(tmpRemainder);
    return quotient;
#endif
}

namespace {

constexpr std::size_t PARTS = 4; // Number of 64-bit parts in 256 bits
constexpr std::size_t BITS_PER_PART = 64;
constexpr std::size_t TOTAL_BITS = PARTS * BITS_PER_PART;

} // anonymous namespace

class uint256_t {
private:
    std::array<uint64_t, PARTS> parts; // parts[0] is the least significant 64 bits
public:
    constexpr uint256_t() : parts{0, 0, 0, 0} {};
    constexpr uint256_t(uint64_t value) : parts{value, 0, 0, 0} {};
    constexpr uint256_t(const uint256_t& other) : parts{other.parts[0], other.parts[1], other.parts[2], other.parts[3]} {};

    constexpr uint256_t& operator+=(const uint256_t& other) {
        uint64_t carry = 0;
        for (std::size_t i = 0; i < PARTS; ++i) {
            uint64_t myPart = parts[i];
            uint64_t otherPart = other.parts[i];

            #ifdef USE_TERNARY_ADDITION

            uint64_t overflowSum = myPart + otherPart + carry;
            carry = (overflowSum < myPart || (carry && overflowSum == myPart)) ? 1 : 0;
            parts[i] = overflowSum;

            #else

            uint64_t overflowSum = myPart + otherPart;
            bool overflowCarry = overflowSum < myPart;
            uint64_t sum = overflowSum + carry;
            bool sumCarry = sum < overflowSum;
            carry = overflowCarry || sumCarry;
            parts[i] = sum;

            #endif // USE_TERNARY_ADDITION
        }

        return *this;
    }

    constexpr uint256_t& operator-=(const uint256_t& other) {
        uint64_t borrow = 0;
        for (std::size_t i = 0; i < PARTS; ++i) {
            uint64_t myPart = parts[i];
            uint64_t otherPart = other.parts[i];

            #ifdef USE_TERNARY_SUBTRACTION

            uint64_t underflowDiff = myPart - otherPart - borrow;
            borrow = (myPart < otherPart || (borrow && underflowDiff >= myPart)) ? 1 : 0;
            parts[i] = underflowDiff;

            #else

            uint64_t underflowDiff = myPart - otherPart;
            bool underflowBorrow = myPart < otherPart;
            uint64_t diff = underflowDiff - borrow;
            bool diffBorrow = underflowDiff < borrow;
            borrow = underflowBorrow || diffBorrow;
            parts[i] = diff;

            #endif // USE_TERNARY_SUBTRACTION
        }

        return *this;
    }

    constexpr uint256_t& operator*=(const uint256_t& other) {
        uint256_t result = 0;
        uint256_t temp_this = *this;

        for (std::size_t i = 0; i < PARTS; ++i) {
            if (other.parts[i] != 0) {
                uint256_t term = temp_this;
                term *= other.parts[i];
                result += term;
            }
            temp_this <<= 64;
        }
        *this = result;
        return *this;
    }

    // 64-bit scalar mutiplication used in 256-bit multiplication
    constexpr uint256_t& operator*=(uint64_t scalar) {

        #ifdef SUPPORTS_UINT128_EXTENSION

        unsigned __int128 carry = 0;
        for (std::size_t i = 0; i < PARTS; ++i) {
            unsigned __int128 product = (unsigned __int128)this->parts[i] * scalar + carry;
            this->parts[i] = (uint64_t)product;
            carry = product >> 64;
        }

        #else

        uint256_t temp = *this;
        *this = 0;
        for (int i = 0; i < 64; ++i) {
            if ((scalar >> i) & 1) {
                *this += (temp << i);
            }
        }

        #endif // SUPPORTS_UINT128_EXTENSION

        return *this;
    }

    constexpr uint256_t& operator<<=(uint32_t shiftBits) {
        if (shiftBits == 0) return *this;
        if (shiftBits >= 256) {
            *this = 0;
            return *this;
        }

        // Shift in increments of 64 bits
        const uint32_t upperPartShifts = shiftBits / 64;
        const uint32_t lowerPartShift = shiftBits % 64;
        if (upperPartShifts > 0) {
            for (uint32_t i = 3; i >= upperPartShifts; --i) {
                parts[i] = parts[i - upperPartShifts];
            }
            for (uint32_t i = 0; i < upperPartShifts; ++i) {
                parts[i] = 0;
            }
        }
        if (lowerPartShift > 0) {
            for (int i = 3; i > 0; --i) {
                parts[i] = (parts[i] << lowerPartShift) | (parts[i - 1] >> (64 - lowerPartShift));
            }
            parts[0] <<= lowerPartShift;
        }

        return *this;
    }

    constexpr uint256_t& operator>>=(uint32_t shiftBits) {
        if (shiftBits == 0) return *this;
        if (shiftBits >= 256) {
            *this = 0;
            return *this;
        }

        // Shift in increments of 64 bits
        const uint32_t lowerPartShifts = shiftBits / 64;
        const uint32_t upperPartShift = shiftBits % 64;
        if (lowerPartShifts > 0) {
            for (uint32_t i = 0; i <= 3 - lowerPartShifts; ++i) {
                parts[i] = parts[i + lowerPartShifts];
            }
            for (uint32_t i = 4 - lowerPartShifts; i < 4; ++i) {
                parts[i] = 0;
            }
        }
        if (upperPartShift > 0) {
            for (uint32_t i = 0; i < 3; ++i) {
                parts[i] = (parts[i] >> upperPartShift) | (parts[i + 1] << (64 - upperPartShift));
            }
            parts[3] >>= upperPartShift;
        }

        return *this;
    }

    constexpr uint256_t& operator&=(const uint256_t& other) {
        for (std::size_t i = 0; i < PARTS; ++i) {
            parts[i] &= other.parts[i];
        }
        return *this;
    }

    bool operator<(const uint256_t& other) const {
        for (int i = static_cast<int>(PARTS) - 1; i >= 0; --i) {
            if (parts[static_cast<std::size_t>(i)] < other.parts[static_cast<std::size_t>(i)]) return true;
            if (parts[static_cast<std::size_t>(i)] > other.parts[static_cast<std::size_t>(i)]) return false;
        }
        return false;
    }

    bool operator==(const uint256_t& other) const {
        return parts == other.parts;
    }

    bool operator!=(const uint256_t& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const uint256_t& value);

};

inline constexpr uint256_t operator+(uint256_t lhs, const uint256_t& rhs) { return lhs += rhs; }
inline constexpr uint256_t operator-(uint256_t lhs, const uint256_t& rhs) { return lhs -= rhs; }
inline constexpr uint256_t operator*(uint256_t lhs, const uint256_t& rhs) { return lhs *= rhs; }
inline constexpr uint256_t operator*(uint256_t lhs, uint64_t rhs) { return lhs *= rhs; }
inline constexpr uint256_t operator<<(uint256_t lhs, uint32_t rhs) { return lhs <<= rhs; }
inline constexpr uint256_t operator>>(uint256_t lhs, uint32_t rhs) { return lhs >>= rhs; }
inline constexpr uint256_t operator&(uint256_t lhs, const uint256_t& rhs) { return lhs &= rhs; }
inline std::ostream& operator<<(std::ostream& os, const uint256_t& value) {
    // Convert to decimal string representation
    if (value == uint256_t(0)) {
        os << '0';
        return os;
    }

    uint256_t temp = value;
    std::string result;
    result.reserve(78); // Max digits

    while (temp != uint256_t(0)) {

        uint64_t remainder = 0;

        #ifdef SUPPORTS_UINT128_EXTENSION

        for (int i = PARTS - 1; i >= 0; --i) {
            unsigned __int128 part = (static_cast<unsigned __int128>(remainder) << 64) | temp.parts[static_cast<std::size_t>(i)];
            temp.parts[static_cast<std::size_t>(i)] = static_cast<uint64_t>(part / 10);
            remainder = static_cast<uint64_t>(part % 10);
        }

        #else

        uint32_t rem = 0;
        for (int i = static_cast<int>(PARTS) - 1; i >= 0; --i) {
            auto idx = static_cast<std::size_t>(i);
            uint64_t q = div10_u64_no128(temp.parts[idx], rem);
            temp.parts[idx] = q;
        }
        remainder = rem;

        #endif // SUPPORTS_UINT128_EXTENSION

        result.push_back(static_cast<char>('0' + remainder));
    }
    std::reverse(result.begin(), result.end());
    os << result;
    return os;
}

#endif // UINT256_T_HPP
