//
// Created by djumi on 1/29/2024.
//

#include <cstdint>
#include <cstring>
#include "utility.h"

namespace utility {
    bool is_valid_literal(uint32_t literal) {
        int32_t value;
        std::memcpy(&value, &literal, sizeof(int32_t));
        constexpr int32_t min = -(1 << 12);
        constexpr int32_t max = (1 << 12) - 1;
        return min <= value && value <= max;
    }
}