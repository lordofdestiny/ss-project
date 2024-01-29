//
// Created by djumi on 11/22/2023.
//

#include "ast/stmt_t.h"

namespace m_asm::ast {
    stmt_t::visitor_t::~visitor_t() {}

    std::array<std::string, 26> instruction_names{
            "HALT",
            "INT", "IRET",
            "CALL", "RET",
            "JMP", "BEQ", "BNE", "BGT",
            "PUSH", "POP",
            "XCHG",
            "ADD", "SUB", "MUL", "DIV",
            "NOT", "AND", "OR", "XOR",
            "SHL", "SHR",
            "LD", "ST",
            "CSRRD", "CSRWR"
    };


    std::ostream &operator<<(std::ostream &os, stmt_t::instr_t::mnemonic_t t) {
        return os << instruction_names[static_cast<int>(t)];
    }

    std::ostream &operand::operator<<(std::ostream &os, operand::reg_t reg) {
        if (reg.is_csr) {
            switch (reg) {
            case 0: return os << "%status";
            case 1: return os << "%handler";
            case 2: return os << "%cause";
            }
        }
        switch (reg) {
        case 14:return os << "%sp";
        case 15:return os << "%pc";
        default:return os << "%r" << +reg.value;
        }
    }

    std::ostream &operand::operator<<(std::ostream &os, operand::operand_t &op) {
        using operand_type_t = operand::operand_t::type_t;
        if (op.type == operand_type_t::LITERAL_VALUE) {
            os << std::get<uint32_t>(op.value);
        } else if (op.type == operand_type_t::SYMBOL_VALUE) {
            os << "VALUE(" << std::get<std::string>(op.value) << ")";
        } else if (op.type == operand_type_t::LITERAL_ADDR) {
            os << "MEM[" << std::get<uint32_t>(op.value) << "]";
        } else if (op.type == operand_type_t::SYMBOL_ADDR) {
            os << "MEM[" << std::get<std::string>(op.value) << "]";
        } else if (op.type == operand_type_t::REG_VALUE) {
            os << std::get<operand::reg_t>(op.value) << "";
        } else if (op.type == operand_type_t::REG_MEMORY_ADDR) {
            os << "MEM[" << std::get<operand::reg_t>(op.value) << "]";
        } else if (op.type == operand_type_t::REG_ADDR_LITERAL_OFFSET) {
            auto &&[reg, offset] = std::get<operand::reg_literal_t>(op.value);
            os << "[" << reg << " + " << offset << "]";
        } else if (op.type == operand_type_t::REG_ADDR_SYMBOL_OFFSET) {
            auto &&[reg, symbol] = std::get<operand::reg_symbol_t>(op.value);
            os << "[" << reg << " + " << symbol << "]";
        }
        return os;
    }
} // ast