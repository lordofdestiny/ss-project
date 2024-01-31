//
// Created by djumi on 1/29/2024.
//

#include <cstdint>
#include <cstring>
#include <string>
#include "utility.h"

namespace utility {
    bool is_valid_literal(const uint32_t literal) {
        int32_t value;
        std::memcpy(&value, &literal, sizeof(int32_t));
        constexpr int32_t min = -(1 << 12);
        constexpr int32_t max = (1 << 12) - 1;
        return min <= value && value <= max;
    }

    std::string hex_to_string(uint32_t hex) {
        char buffer[9];
        for (int i = 7; i >= 0; i--) {
            const auto digit = hex & 0xF;
            buffer[i] = static_cast<char>(digit + (digit < 10 ? '0' : 'A' - 10));
            hex >>= 4;
        }
        return buffer;
    }
}