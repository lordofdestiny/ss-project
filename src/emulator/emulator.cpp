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

    void emulate(context_t &context) {
        using namespace std::string_literals;
        bool done = false;
        while (!done) {
            const auto instr = context.read_instr();
            switch (static_cast<instr_code>(instr.code)) {
            case HALT: {
                done = true;
                break;
            }
            case INT: {
                // push status
                context.sp -= 4;
                context.write_to(context.sp, context.status);
                // push pc
                context.sp -= 4;
                context.write_to(context.sp, context.pc);

                context.cause = 4;
                context.status = context.status & ~0x1;
                context.pc = context.handler;
                break;
            }
            case CALL: {
                using call_mode = common::instruction_t::call_mode;
                switch (static_cast<call_mode>(instr.mode)) {
                case call_mode::REG_IND_DISP:
                    context.sp -= 4;
                    context.write_to(context.sp, context.pc);
                    context.pc = context.gpr[instr.reg1] + context.gpr[instr.reg2] + instr.disp;
                    break;
                case call_mode::MEM_IND_DISP:
                    context.sp -= 4;
                    context.write_to(context.sp, context.pc);
                    context.pc = context.read_word(
                        context.gpr[instr.reg1] + context.gpr[instr.reg2] + instr.disp);
                    break;
                default: {
                    std::stringstream ss;
                    ss << "Invalid mode for call instruction: " << instr.mode;
                    throw std::logic_error(ss.str());
                }
                }
                break;
            }
            case JMP: {
                using jump_mode = common::instruction_t::jump_mode;
                switch (static_cast<jump_mode>(instr.mode)) {
                case jump_mode::REG_DIR_DISP:
                    context.pc = context.gpr[instr.reg1] + instr.disp;
                    break;
                case jump_mode::EQ_REG_DIR_DISP:
                    if (context.gpr[instr.reg2] == context.gpr[instr.reg3]) {
                        context.pc = context.gpr[instr.reg1] + instr.disp;
                    }
                    break;
                case jump_mode::NE_REG_DIR_DISP:
                    if (context.gpr[instr.reg2] != context.gpr[instr.reg3]) {
                        context.pc = context.gpr[instr.reg1] + instr.disp;
                    }
                    break;
                case jump_mode::GT_REG_DIR_DISP:
                    if (static_cast<int32_t>(context.gpr[instr.reg2])
                        > static_cast<int32_t>(context.gpr[instr.reg3])) {
                        context.pc = context.gpr[instr.reg1] + instr.disp;
                    }
                    break;
                case jump_mode::REG_IND_DISP:
                    context.pc = context.read_word(context.gpr[instr.reg1] + instr.disp);
                    break;
                case jump_mode::EQ_REG_IND_DISP:
                    if (context.gpr[instr.reg2] == context.gpr[instr.reg3]) {
                        context.pc = context.read_word(context.gpr[instr.reg1] + instr.disp);
                    }
                    break;
                case jump_mode::NE_REG_IND_DISP:
                    if (context.gpr[instr.reg2] != context.gpr[instr.reg3]) {
                        context.pc = context.read_word(context.gpr[instr.reg1] + instr.disp);
                    }
                    break;
                case jump_mode::GT_REG_IND_DISP:
                    if (static_cast<int32_t>(context.gpr[instr.reg2])
                        > static_cast<int32_t>(context.gpr[instr.reg3])) {
                        context.pc = context.read_word(context.gpr[instr.reg1] + instr.disp);
                    }
                    break;
                default: {
                    std::stringstream ss;
                    ss << "Invalid mode for jump instruction: " << instr.mode;
                    throw std::logic_error(ss.str());
                }
                }
                break;
            }
            case XCHG: {
                std::swap(context.gpr[instr.reg2], context.gpr[instr.reg3]);
                break;
            }
            case MATH: {
                using arithmetic_mode = common::instruction_t::arithmetic_mode;
                switch (static_cast<arithmetic_mode>(instr.mode)) {
                case arithmetic_mode::ADD:
                    context.gpr[instr.reg1] = context.gpr[instr.reg2] + context.gpr[instr.reg3];
                    break;
                case arithmetic_mode::SUB:
                    context.gpr[instr.reg1] = context.gpr[instr.reg2] - context.gpr[instr.reg3];
                    break;
                case arithmetic_mode::MUL:
                    context.gpr[instr.reg1] = context.gpr[instr.reg2] * context.gpr[instr.reg3];
                    break;
                case arithmetic_mode::DIV:
                    context.gpr[instr.reg1] = context.gpr[instr.reg2] / context.gpr[instr.reg3];
                    break;
                default: {
                    std::stringstream ss;
                    ss << "Invalid mode for arithmetic instruction: " << instr.mode;
                    throw std::logic_error(ss.str());
                }
                }
                break;
            }
            case LOGIC: {
                using logic_mode = common::instruction_t::logic_mode;
                switch (static_cast<logic_mode>(instr.mode)) {
                case logic_mode::NOT:
                    context.gpr[instr.reg1] = ~context.gpr[instr.reg2];
                    break;
                case logic_mode::AND:
                    context.gpr[instr.reg1] = context.gpr[instr.reg2] & context.gpr[instr.reg3];
                    break;
                case logic_mode::OR:
                    context.gpr[instr.reg1] = context.gpr[instr.reg2] | context.gpr[instr.reg3];
                    break;
                case logic_mode::XOR:
                    context.gpr[instr.reg1] = context.gpr[instr.reg2] ^ context.gpr[instr.reg3];
                    break;
                default: {
                    std::stringstream ss;
                    ss << "Invalid mode for logic instruction: " << instr.mode;
                    throw std::logic_error(ss.str());
                }
                }
                break;
            }
            case SHIFT: {
                using shift_mode = common::instruction_t::shift_mode;
                switch (static_cast<shift_mode>(instr.mode)) {
                    break;
                case shift_mode::LEFT:
                    context.gpr[instr.reg1] = context.gpr[instr.reg2] << context.gpr[instr.reg3];
                    break;
                case shift_mode::RIGHT:
                    context.gpr[instr.reg1] = context.gpr[instr.reg2] >> context.gpr[instr.reg3];
                    break;
                default: {
                    std::stringstream ss;
                    ss << "Invalid mode for shift instruction: " << instr.mode;
                    throw std::logic_error(ss.str());
                }
                }
                break;
            }
            case STORE: {
                using store_mode = common::instruction_t::store_mode;
                switch (static_cast<store_mode>(instr.mode)) {
                case store_mode::REG_DISP_IND: {
                    context.write_to(context.gpr[instr.reg1] + context.gpr[instr.reg2] + instr.disp,
                                     context.gpr[instr.reg3]);
                    break;
                }
                case store_mode::MEM_REG_DISP_IND: {
                    const auto location_address = context.gpr[instr.reg1] + context.gpr[instr.reg2] + instr.disp;
                    const auto store_address = context.read_word(location_address);
                    context.write_to(store_address, context.gpr[instr.reg3]);
                    break;
                }
                case store_mode::REG_IND_PREINC: {
                    context.gpr[instr.reg1] = context.gpr[instr.reg1] + instr.disp;
                    context.write_to(context.gpr[instr.reg1], context.gpr[instr.reg3]);
                    break;
                }
                default: {
                    std::stringstream ss;
                    ss << "Invalid mode for store instruction: " << instr.mode;
                    throw std::logic_error(ss.str());
                }
                }
                break;
            }
            case LOAD: {
                using load_mode = common::instruction_t::load_mode;
                switch (static_cast<load_mode>(instr.mode)) {
                case load_mode::READ_CSR:
                    context.gpr[instr.reg1] = context.csr[instr.reg2];
                    break;
                case load_mode::REG_MOVE_INC:
                    context.gpr[instr.reg1] = context.gpr[instr.reg2] + instr.disp;
                    break;
                case load_mode::REG_DISP_IND:
                    context.gpr[instr.reg1] = context.read_word(
                        context.gpr[instr.reg2] + context.gpr[instr.reg3] + instr.disp
                    );
                    break;
                case load_mode::REG_DISP_IND_POSTINC:
                    context.gpr[instr.reg1] = context.read_word(context.gpr[instr.reg2]);
                    context.gpr[instr.reg2] = context.gpr[instr.reg2] + instr.disp;
                    break;
                case load_mode::WRITE_CSR:
                    context.csr[instr.reg1] = context.gpr[instr.reg2];
                    break;
                case load_mode::WRITE_CSR_MOVE_OR:
                    context.csr[instr.reg1] = context.csr[instr.reg2] | instr.disp;
                    break;
                case load_mode::WRITE_CSR_REG_DISP_IND:
                    context.csr[instr.reg1] = context.read_word(
                        context.gpr[instr.reg2] + context.gpr[instr.reg3] + instr.disp
                    );
                    break;
                case load_mode::WRITE_CSR_REG_DISP_IND_POSTINC:
                    context.csr[instr.reg1] = context.read_word(context.gpr[instr.reg2]);
                    context.gpr[instr.reg2] = context.gpr[instr.reg2] + instr.disp;
                    break;
                default: {
                    std::stringstream ss;
                    ss << "Invalid mode for load instruction: " << instr.mode;
                    throw std::logic_error(ss.str());
                }
                }
                break;
            }
            default: {
                std::stringstream ss;
                ss << "Unknow opcode: " << instr.code;
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
