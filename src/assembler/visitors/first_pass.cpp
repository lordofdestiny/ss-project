//
// Created by djumi on 1/29/2024.
//

#include <exception>

#include <algorithm>
#include "visitors/first_pass.h"

namespace m_asm::visitor {
    void first_pass::visit_label(stmt_t::label_t &label) {
        asm_ref.get().add_symbol(
            true, label.label, asm_ref.get().current_section().index,
            asm_ref.get().current_section().size, true, 'l');
    }

    void first_pass::visit_global(stmt_t::global_t &global) {
        auto &symtab = asm_ref.get().get_symbol_table();
        for (const auto &symbol_name: global.symbols) {
            if (const auto symbol = symtab.find(symbol_name)) {
                symbol->local = 'g';
            }
        }
    }

    void first_pass::visit_extern(stmt_t::extern_t &anExtern) {
        for (const auto &symbol: anExtern.symbols) {
            asm_ref.get().add_symbol(
                false, symbol,
                common::symbol::section_t::SECTION_UNDEF,
                0, false, 'g');
        }
    }

    void first_pass::visit_section(stmt_t::section_t &section) {
        asm_ref.get().create_section(section.name);
    }

    void first_pass::visit_word(stmt_t::word_t &word) {
        asm_ref.get().increment_section_size(4 * word.word_values.size());
    }

    void first_pass::visit_skip(stmt_t::skip_t &skip) {
        asm_ref.get().increment_section_size(skip.size);
    }

    void first_pass::visit_ascii(stmt_t::ascii_t &ascii) {
        asm_ref.get().increment_section_size(ascii.value.size() + 1);
    }

    void first_pass::visit_equals(stmt_t::equals_t &) {
        throw std::logic_error("equals directive not implemented!");
    }

    void first_pass::visit_paramless_instr(stmt_t::instr_t::paramless_t &paramless) {
        if (paramless.type == mnemonic_t::IRET) {
            asm_ref.get().increment_section_size(8);
        } else {
            asm_ref.get().increment_section_size(4);
        }
    }

    void first_pass::visit_unary_instr(stmt_t::instr_t::unary_t &) {
        asm_ref.get().increment_section_size(4);
    }

    void first_pass::visit_binary_instr(stmt_t::instr_t::binary_t &) {
        asm_ref.get().increment_section_size(4);
    }

    void first_pass::visit_memory_instr(stmt_t::instr_t::memory_t &memory) {
        if (memory.type == mnemonic_t::LD) {
            switch (memory.operand.type) {
            case operand_t::type_t::REG_VALUE:
            case operand_t::type_t::REG_MEMORY_ADDR:
            case operand_t::type_t::REG_ADDR_LITERAL_OFFSET:
            case operand_t::type_t::REG_ADDR_SYMBOL_OFFSET:
                asm_ref.get().increment_section_size(4);
                break;
            case operand_t::type_t::LITERAL_VALUE:
            case operand_t::type_t::SYMBOL_VALUE:
                asm_ref.get().increment_section_size(12);
                break;
            case operand_t::type_t::LITERAL_ADDR:
            case operand_t::type_t::SYMBOL_ADDR:
                asm_ref.get().increment_section_size(16);
                break;
            }
            return;
        }
        if (memory.type == mnemonic_t::ST) {
            switch (memory.operand.type) {
            case operand_t::type_t::LITERAL_VALUE:
            case operand_t::type_t::SYMBOL_VALUE:
                throw std::logic_error("invalid addressing mode");
            case operand_t::type_t::REG_VALUE:
            case operand_t::type_t::REG_MEMORY_ADDR:
            case operand_t::type_t::REG_ADDR_LITERAL_OFFSET:
            case operand_t::type_t::REG_ADDR_SYMBOL_OFFSET:
                asm_ref.get().increment_section_size(4);
                break;
            case operand_t::type_t::LITERAL_ADDR:
            case operand_t::type_t::SYMBOL_ADDR:
                asm_ref.get().increment_section_size(12);
                break;
            }
            return;
        }
        throw std::logic_error("invalid instruction type");
    }

    void first_pass::visit_uncond_instr(stmt_t::instr_t::uncond_t &) {
        asm_ref.get().increment_section_size(12);
    }

    void first_pass::visit_branch_instr(stmt_t::instr_t::branch_t &) {
        asm_ref.get().increment_section_size(12);
    }
} // m_asm::visitor
