//
// Created by djumi on 11/22/2023.
//

#include "ast/stmt_t.h"

namespace m_asm::ast {
    stmt_t::visitor_t::~visitor_t() {
    }

    std::array<const char *, 26> instruction_names{
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
} // ast
