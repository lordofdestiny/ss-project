//
// Created by djumi on 2/1/2024.
//

#include <type_traits>
#include <cstdint>
#include "common/instruction_t.h"

namespace common {
    instruction_t instruction_t::make_halt() {
        return {0, 0, 0, 0, 0, 0};
    }

    instruction_t instruction_t::make_int() {
        return {1, 0, 0, 0, 0, 0};
    }

    instruction_t instruction_t::make_call(call_mode mode,
                                           const operand::reg_t reg_a,
                                           const operand::reg_t reg_b,
                                           const int16_t disp) {
        return {
            0x2, static_cast<std::underlying_type_t<call_mode>>(mode),
            reg_a, reg_b, 0, disp
        };
    }

    instruction_t instruction_t::make_jump(jump_mode mode,
                                           const operand::reg_t reg_a,
                                           const operand::reg_t reg_b,
                                           const operand::reg_t reg_c,
                                           const int16_t disp) {
        return {
            0x3, static_cast<std::underlying_type_t<jump_mode>>(mode),
            reg_a, reg_b, reg_c, disp
        };
    }

    instruction_t instruction_t::make_xchg(const operand::reg_t reg_b,
                                           const operand::reg_t reg_c) {
        return {0x4, 0, 0, reg_b, reg_c, 0};
    }

    instruction_t instruction_t::make_arithmetic(arithmetic_mode mode,
                                                 const operand::reg_t reg_a,
                                                 const operand::reg_t reg_b,
                                                 const operand::reg_t reg_c) {
        return {
            0x5, static_cast<std::underlying_type_t<arithmetic_mode>>(mode),
            reg_a, reg_b, reg_c, 0
        };
    }

    instruction_t instruction_t::make_logic(logic_mode mode,
                                            const operand::reg_t reg_a,
                                            const operand::reg_t reg_b,
                                            const operand::reg_t reg_c) {
        return {
            0x6, static_cast<std::underlying_type_t<logic_mode>>(mode),
            reg_a, reg_b, reg_c, 0
        };
    }

    instruction_t instruction_t::make_shift(shift_mode mode,
                                            const operand::reg_t reg_a,
                                            const operand::reg_t reg_b,
                                            const operand::reg_t reg_c) {
        return {
            0x7, static_cast<std::underlying_type_t<shift_mode>>(mode),
            reg_a, reg_b, reg_c, 0
        };
    }

    instruction_t instruction_t::make_store(store_mode mode,
                                            const operand::reg_t reg_a,
                                            const operand::reg_t reg_b,
                                            const operand::reg_t reg_c,
                                            const int16_t disp) {
        return {
            0x8, static_cast<std::underlying_type_t<store_mode>>(mode),
            reg_a, reg_b, reg_c, disp
        };
    }

    instruction_t instruction_t::make_load(load_mode mode,
                                           const operand::reg_t reg_a,
                                           const operand::reg_t reg_b,
                                           const operand::reg_t reg_c,
                                           const int16_t disp) {
        return {
            0x9, static_cast<std::underlying_type_t<load_mode>>(mode),
            reg_a, reg_b, reg_c, disp
        };
    }
} // common
