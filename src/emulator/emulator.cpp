//
// Created by djumi on 2/5/2024.
//

#include <iostream>
#include <sstream>

#include "../../include/emulator/context_t.h"

namespace m_emu {
    enum instr_code : uint8_t {
        HALT = 0b0000,
        INT = 0b0001,
        CALL = 0b0010,
        JMP = 0b0011,
        XCHG = 0b0100,
        MATH = 0b0101,
        LOGIC = 0b0110,
        SHIFT = 0b0111,
        STORE = 0b1000,
        LOAD = 0b1001,
        OTHER
    };

    void handle_interrupt(context_t &context, const uint32_t cause) {
        // If interrupts are masked
        if (context.cause & 0b100) return;

        // push status
        context.sp -= 4;
        context.write_to(context.sp, context.status);
        // push pc
        context.sp -= 4;
        context.write_to(context.sp, context.pc);

        context.cause = cause;
        context.status |= 0x100;
        context.pc = context.handler;
    }

    void emulate(context_t &context) {
        using namespace std::string_literals;
        bool done = false;
        while (!done) {
            const auto [code, mode, reg_a, reg_b, reg_c, disp] = context.read_instr();
            switch (static_cast<instr_code>(code)) {
            case HALT: {
                done = true;
                break;
            }
            case INT: {
                handle_interrupt(context, 4);
                break;
            }
            case CALL: {
                using call_mode = common::instruction_t::call_mode;
                switch (static_cast<call_mode>(mode)) {
                case call_mode::REG_IND_DISP:
                    context.sp -= 4;
                    context.write_to(context.sp, context.pc);
                    context.pc = context.gpr[reg_a] + context.gpr[reg_b] + disp;
                    break;
                case call_mode::MEM_IND_DISP:
                    context.sp -= 4;
                    context.write_to(context.sp, context.pc);
                    context.pc = context.read_word(
                        context.gpr[reg_a] + context.gpr[reg_b] + disp);
                    break;
                default: {
                    std::stringstream ss;
                    ss << "Invalid mode for call instruction: " << mode;
                    throw std::logic_error(ss.str());
                }
                }
                break;
            }
            case JMP: {
                using jump_mode = common::instruction_t::jump_mode;
                switch (static_cast<jump_mode>(mode)) {
                case jump_mode::REG_DIR_DISP:
                    context.pc = context.gpr[reg_a] + disp;
                    break;
                case jump_mode::EQ_REG_DIR_DISP:
                    if (context.gpr[reg_b] == context.gpr[reg_c]) {
                        context.pc = context.gpr[reg_a] + disp;
                    }
                    break;
                case jump_mode::NE_REG_DIR_DISP:
                    if (context.gpr[reg_b] != context.gpr[reg_c]) {
                        context.pc = context.gpr[reg_a] + disp;
                    }
                    break;
                case jump_mode::GT_REG_DIR_DISP:
                    if (static_cast<int32_t>(context.gpr[reg_b])
                        > static_cast<int32_t>(context.gpr[reg_c])) {
                        context.pc = context.gpr[reg_a] + disp;
                    }
                    break;
                case jump_mode::REG_IND_DISP:
                    context.pc = context.read_word(context.gpr[reg_a] + disp);
                    break;
                case jump_mode::EQ_REG_IND_DISP:
                    if (context.gpr[reg_b] == context.gpr[reg_c]) {
                        context.pc = context.read_word(context.gpr[reg_a] + disp);
                    }
                    break;
                case jump_mode::NE_REG_IND_DISP:
                    if (context.gpr[reg_b] != context.gpr[reg_c]) {
                        context.pc = context.read_word(context.gpr[reg_a] + disp);
                    }
                    break;
                case jump_mode::GT_REG_IND_DISP:
                    if (static_cast<int32_t>(context.gpr[reg_b])
                        > static_cast<int32_t>(context.gpr[reg_c])) {
                        context.pc = context.read_word(context.gpr[reg_a] + disp);
                    }
                    break;
                default: {
                    std::stringstream ss;
                    ss << "Invalid mode for jump instruction: " << mode;
                    throw std::logic_error(ss.str());
                }
                }
                break;
            }
            case XCHG: {
                std::swap(context.gpr[reg_b], context.gpr[reg_c]);
                break;
            }
            case MATH: {
                using arithmetic_mode = common::instruction_t::arithmetic_mode;
                switch (static_cast<arithmetic_mode>(mode)) {
                case arithmetic_mode::ADD:
                    context.gpr[reg_a] = context.gpr[reg_b] + context.gpr[reg_c];
                    break;
                case arithmetic_mode::SUB:
                    context.gpr[reg_a] = context.gpr[reg_b] - context.gpr[reg_c];
                    break;
                case arithmetic_mode::MUL:
                    context.gpr[reg_a] = context.gpr[reg_b] * context.gpr[reg_c];
                    break;
                case arithmetic_mode::DIV:
                    context.gpr[reg_a] = context.gpr[reg_b] / context.gpr[reg_c];
                    break;
                default: {
                    std::stringstream ss;
                    ss << "Invalid mode for arithmetic instruction: " << mode;
                    throw std::logic_error(ss.str());
                }
                }
                break;
            }
            case LOGIC: {
                using logic_mode = common::instruction_t::logic_mode;
                switch (static_cast<logic_mode>(mode)) {
                case logic_mode::NOT:
                    context.gpr[reg_a] = ~context.gpr[reg_b];
                    break;
                case logic_mode::AND:
                    context.gpr[reg_a] = context.gpr[reg_b] & context.gpr[reg_c];
                    break;
                case logic_mode::OR:
                    context.gpr[reg_a] = context.gpr[reg_b] | context.gpr[reg_c];
                    break;
                case logic_mode::XOR:
                    context.gpr[reg_a] = context.gpr[reg_b] ^ context.gpr[reg_c];
                    break;
                default: {
                    std::stringstream ss;
                    ss << "Invalid mode for logic instruction: " << mode;
                    throw std::logic_error(ss.str());
                }
                }
                break;
            }
            case SHIFT: {
                using shift_mode = common::instruction_t::shift_mode;
                switch (static_cast<shift_mode>(mode)) {
                case shift_mode::LEFT:
                    context.gpr[reg_a] = context.gpr[reg_b] << context.gpr[reg_c];
                    break;
                case shift_mode::RIGHT:
                    context.gpr[reg_a] = context.gpr[reg_b] >> context.gpr[reg_c];
                    break;
                default: {
                    std::stringstream ss;
                    ss << "Invalid mode for shift instruction: " << mode;
                    throw std::logic_error(ss.str());
                }
                }
                break;
            }
            case STORE: {
                using store_mode = common::instruction_t::store_mode;
                switch (static_cast<store_mode>(mode)) {
                case store_mode::REG_DISP_IND: {
                    context.write_to(context.gpr[reg_a] + context.gpr[reg_b] + disp,
                                     context.gpr[reg_c]);
                    break;
                }
                case store_mode::MEM_REG_DISP_IND: {
                    const auto location_address = context.gpr[reg_a] + context.gpr[reg_b] + disp;
                    const auto store_address = context.read_word(location_address);
                    context.write_to(store_address, context.gpr[reg_c]);
                    break;
                }
                case store_mode::REG_IND_PREINC: {
                    context.gpr[reg_a] = context.gpr[reg_a] + disp;
                    context.write_to(context.gpr[reg_a], context.gpr[reg_c]);
                    break;
                }
                default: {
                    std::stringstream ss;
                    ss << "Invalid mode for store instruction: " << mode;
                    throw std::logic_error(ss.str());
                }
                }
                break;
            }
            case LOAD: {
                using load_mode = common::instruction_t::load_mode;
                switch (static_cast<load_mode>(mode)) {
                case load_mode::READ_CSR:
                    context.gpr[reg_a] = context.csr[reg_b];
                    break;
                case load_mode::REG_MOVE_INC:
                    context.gpr[reg_a] = context.gpr[reg_b] + disp;
                    break;
                case load_mode::REG_DISP_IND:
                    context.gpr[reg_a] = context.read_word(
                        context.gpr[reg_b] + context.gpr[reg_c] + disp
                    );
                    break;
                case load_mode::REG_DISP_IND_POSTINC:
                    context.gpr[reg_a] = context.read_word(context.gpr[reg_b]);
                    context.gpr[reg_b] = context.gpr[reg_b] + disp;
                    break;
                case load_mode::WRITE_CSR:
                    context.csr[reg_a] = context.gpr[reg_b];
                    break;
                case load_mode::WRITE_CSR_MOVE_OR:
                    context.csr[reg_a] = context.csr[reg_b] | disp;
                    break;
                case load_mode::WRITE_CSR_REG_DISP_IND:
                    context.csr[reg_a] = context.read_word(
                        context.gpr[reg_b] + context.gpr[reg_c] + disp
                    );
                    break;
                case load_mode::WRITE_CSR_REG_DISP_IND_POSTINC:
                    context.csr[reg_a] = context.read_word(context.gpr[reg_b]);
                    context.gpr[reg_b] = context.gpr[reg_b] + disp;
                    break;
                default: {
                    std::stringstream ss;
                    ss << "Invalid mode for load instruction: " << mode;
                    throw std::logic_error(ss.str());
                }
                }
                break;
            }
            default: {
                std::stringstream ss;
                ss << "Unknow opcode: " << code;
                ss << "; pc = " << std::hex << context.pc - 4;
                throw std::logic_error(ss.str());
            }
            }

            // Override writes to r0
            context.gpr[0] = 0;
            std::cout << context << '\n';
        }
    }
}
