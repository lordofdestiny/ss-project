//
// Created by djumi on 2/1/2024.
//

#ifndef INSTRUCTION_T_H
#define INSTRUCTION_T_H

#include <cstdint>
#include <type_traits>

#include "operand.h"
#include "common/operand.h"

namespace common {
    struct instruction_t {
        uint32_t code: 4;
        uint32_t mode: 4;
        uint32_t reg1: 4;
        uint32_t reg2: 4;
        uint32_t reg3: 4;
        int32_t disp: 12;

        enum struct call_mode : uint8_t {
            REG_IND_DISP = 0,
            MEM_IND_DISP = 1
        };

        enum class jump_mode : uint8_t {
            REG_DIR_DISP = 0,
            EQ_REG_DIR_DISP = 1,
            NE_REG_DIR_DISP = 2,
            GT_REG_DIR_DISP = 3,
            REG_IND_DISP = 8,
            EQ_REG_IND_DISP = 9,
            NE_REG_IND_DISP = 10,
            GT_REG_IND_DISP = 11
        };

        enum class arithmetic_mode : uint8_t {
            ADD = 0,
            SUB = 1,
            MUL = 2,
            DIV = 3
        };

        enum class logic_mode : uint8_t {
            NOT = 0,
            AND = 1,
            OR = 2,
            XOR = 3
        };

        enum class shift_mode : uint8_t {
            LEFT = 0,
            RIGHT = 1
        };

        enum class store_mode : uint8_t {
            REG_DISP_IND = 0,
            MEM_REG_DISP_IND = 2,
            REG_IND_PREINC = 1,
        };

        enum class load_mode : uint8_t {
            READ_CSR = 0,
            REG_MOVE_INC = 1,
            REG_DISP_IND = 2,
            REG_DISP_IND_POSTINC = 3,
            WRITE_CSR = 4,
            WRITE_CSR_MOVE_OR = 5,
            WRITE_CSR_REG_DISP_IND = 6,
            WRITE_CSR_REG_DISP_IND_POSTINC = 7
        };

        static instruction_t make_halt();

        static instruction_t make_int();

        static instruction_t make_call(call_mode mode,
                                       operand::reg_t reg_a,
                                       operand::reg_t reg_b,
                                       int16_t disp);

        static instruction_t make_jump(jump_mode mode,
                                       operand::reg_t reg_a,
                                       operand::reg_t reg_b,
                                       operand::reg_t reg_c,
                                       int16_t disp);

        static instruction_t make_xchg(operand::reg_t reg_b,
                                       operand::reg_t reg_c);

        static instruction_t make_arithmetic(arithmetic_mode mode,
                                             operand::reg_t reg_a,
                                             operand::reg_t reg_b,
                                             operand::reg_t reg_c);

        static instruction_t make_logic(logic_mode mode,
                                        operand::reg_t reg_a,
                                        operand::reg_t reg_b,
                                        operand::reg_t reg_c);

        static instruction_t make_shift(shift_mode mode,
                                        operand::reg_t reg_a,
                                        operand::reg_t reg_b,
                                        operand::reg_t reg_c);

        static instruction_t make_store(store_mode mode,
                                        operand::reg_t reg_a,
                                        operand::reg_t reg_b,
                                        operand::reg_t reg_c,
                                        int16_t disp);

        static instruction_t make_load(load_mode mode,
                                       operand::reg_t reg_a,
                                       operand::reg_t reg_b,
                                       operand::reg_t reg_c,
                                       int16_t disp);

        [[nodiscard]] uint32_t to_word() const {
            return (code << 28) | (mode << 24) | (reg1 << 20) | (reg2 << 16) | (reg3 << 12) | (disp & 0xFFF);
        }

        [[nodiscard]] static uint32_t to_negative_12_bit(const uint32_t value) {
            return (~value + 1u) & ~(~0u << 12);
        }
    };

    static_assert(std::is_trivial_v<instruction_t>, "instruction_repr_t must be trivial");
    static_assert(sizeof(instruction_t) == sizeof(uint32_t));
} // common

#endif //INSTRUCTION_T_H
