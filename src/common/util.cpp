//
// Created by djumi on 1/29/2024.
//

#include <cstdint>
#include <cstring>
#include <string>
#include "common/util.h"

namespace common::util {
    bool is_valid_literal(const uint32_t literal) {
        constexpr int32_t min = -(1 << 12);
        constexpr int32_t max = (1 << 12) - 1;
        const auto value = static_cast<int32_t>(literal);
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
