//
// Created by djumi on 1/31/2024.
//

#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <functional>
#include "ast/stmt_t.h"
#include "assembler.h"

namespace m_asm::visitor {
    struct second_pass final : stmt_t::visitor_t {
        explicit second_pass(const std::reference_wrapper<assembler> assembler)
            : asm_ref(assembler) {
        }

        std::reference_wrapper<assembler> asm_ref;

    private:
        void visit_global(stmt_t::global_t &global) override;

        void visit_section(stmt_t::section_t &section) override;

        void visit_word(stmt_t::word_t &word) override;

        void visit_skip(stmt_t::skip_t &skip) override;

        void visit_ascii(stmt_t::ascii_t &ascii) override;

        void visit_equals(stmt_t::equals_t &equals) override;

        void visit_paramless_instr(stmt_t::instr_t::paramless_t &paramless) override;

        void visit_unary_instr(stmt_t::instr_t::unary_t &unary) override;

        void visit_binary_instr(stmt_t::instr_t::binary_t &binary) override;

        void visit_memory_instr(stmt_t::instr_t::memory_t &memory) override;

        void visit_uncond_instr(stmt_t::instr_t::uncond_t &uncond) override;

        void visit_branch_instr(stmt_t::instr_t::branch_t &branch) override;
    };
} // m_asm::visitors

#endif //SECOND_PASS_H
