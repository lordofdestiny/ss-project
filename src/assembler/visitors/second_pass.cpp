//
// Created by djumi on 1/31/2024.
//

#include "visitors/second_pass.h"
#include "ast/stmt_t.h"

namespace m_asm::visitor {
    void second_pass::visit_global([[maybe_unused]] stmt_t::global_t &global) {
    }

    void second_pass::visit_extern([[maybe_unused]] stmt_t::extern_t &anExtern) {
    }

    void second_pass::visit_section([[maybe_unused]] stmt_t::section_t &section) {
    }

    void second_pass::visit_word([[maybe_unused]] stmt_t::word_t &word) {
    }

    void second_pass::visit_skip([[maybe_unused]] stmt_t::skip_t &skip) {
    }

    void second_pass::visit_ascii([[maybe_unused]] stmt_t::ascii_t &ascii) {
    }

    void second_pass::visit_equals([[maybe_unused]] stmt_t::equals_t &equals) {
    }

    void second_pass::visit_paramless_instr([[maybe_unused]] stmt_t::instr_t::paramless_t &paramless) {
    }

    void second_pass::visit_unary_instr([[maybe_unused]] stmt_t::instr_t::unary_t &unary) {
    }

    void second_pass::visit_binary_instr([[maybe_unused]] stmt_t::instr_t::binary_t &binary) {
    }

    void second_pass::visit_memory_instr([[maybe_unused]] stmt_t::instr_t::memory_t &memory) {
    }

    void second_pass::visit_uncond_instr([[maybe_unused]] stmt_t::instr_t::uncond_t &uncond) {
    }

    void second_pass::visit_branch_instr([[maybe_unused]] stmt_t::instr_t::branch_t &branch) {
    }
} // m_asm::visitors
