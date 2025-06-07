//
// Created by djumi on 2/2/2024.
//

#include <ostream>
#include <variant>
#include <tuple>

#include "common/operand.h"

namespace common::operand {
    reg_t reg_t::sp = 14;
    reg_t reg_t::pc = 15;
    reg_t reg_t::status = 0;
    reg_t reg_t::handler = 1;
    reg_t reg_t::cause = 2;

    std::ostream &operator<<(std::ostream &os, const reg_t reg) {
        if (reg.is_csr) {
            switch (reg) {
            case 0: return os << "%status";
            case 1: return os << "%handler";
            case 2: return os << "%cause";
            default: break;
            }
        }
        switch (reg) {
        case 14: return os << "%sp";
        case 15: return os << "%pc";
        default: return os << "%r" << +reg.value;
        }
    }

    std::ostream &operator<<(std::ostream &os, operand_t const &op) {
        using operand_type_t = operand_t::type_t;
        if (op.type == operand_type_t::LITERAL_VALUE) {
            os << std::get<uint32_t>(op.value);
        } else if (op.type == operand_type_t::SYMBOL_VALUE) {
            os << "VALUE(" << std::get<std::string>(op.value) << ")";
        } else if (op.type == operand_type_t::LITERAL_ADDR) {
            os << "MEM[" << std::get<uint32_t>(op.value) << "]";
        } else if (op.type == operand_type_t::SYMBOL_ADDR) {
            os << "MEM[" << std::get<std::string>(op.value) << "]";
        } else if (op.type == operand_type_t::REG_VALUE) {
            os << std::get<reg_t>(op.value) << "";
        } else if (op.type == operand_type_t::REG_MEMORY_ADDR) {
            os << "MEM[" << std::get<reg_t>(op.value) << "]";
        } else if (op.type == operand_type_t::REG_ADDR_LITERAL_OFFSET) {
            auto &&[reg, offset] = std::get<reg_literal_t>(op.value);
            os << "[" << reg << " + " << offset << "]";
        } else if (op.type == operand_type_t::REG_ADDR_SYMBOL_OFFSET) {
            auto &&[reg, symbol] = std::get<reg_symbol_t>(op.value);
            os << "[" << reg << " + " << symbol << "]";
        }
        return os;
    }
}
