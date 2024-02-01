//
// Created by djumi on 1/29/2024.
//

#ifndef SS_ALE_FIRST_PASS_H
#define SS_ALE_FIRST_PASS_H

#include "assembler.h"
#include "ast/stmt_t.h"

namespace m_asm::visitor {
    struct first_pass final : stmt_t::visitor_t {
        explicit first_pass(const std::reference_wrapper<assembler> assembler)
            : asm_ref(assembler) {
        }

        std::reference_wrapper<assembler> asm_ref;

    private:
        void visit_label(stmt_t::label_t &label) override;

        void visit_global(stmt_t::global_t &global) override;

        void visit_extern(stmt_t::extern_t &anExtern) override;

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
} // m_asm

#endif //SS_ALE_FIRST_PASS_H
