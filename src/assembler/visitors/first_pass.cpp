//
// Created by djumi on 1/29/2024.
//

#include <exception>

#include <algorithm>
#include "visitors/first_pass.h"

namespace m_asm::visitor {
    void first_pass::visit_label(stmt_t::label_t &label) {
        assembler.get().add_symbol(
            true, label.label, assembler.get().get_current_section(),
            assembler.get().get_sections().back().size, 'l');
    }

    void first_pass::visit_global(stmt_t::global_t &global) {
        auto symtab = assembler.get().get_symbol_table();
        for (const auto &symbol_name: global.symbols) {
            if (const auto symbol = symtab.find(symbol_name)) {
                symbol->local = 'g';
            }
        }
    }

    void first_pass::visit_extern(stmt_t::extern_t &anExtern) {
        for (const auto &symbol: anExtern.symbols) {
            assembler.get().add_symbol(false, symbol, 0, 0, 'g');
        }
    }

    void first_pass::visit_section(stmt_t::section_t &section) {
        assembler.get().create_section(section.name);
    }

    void first_pass::visit_word(stmt_t::word_t &word) {
        assembler.get().increment_section_size(4 * word.word_values.size());
    }

    void first_pass::visit_skip(stmt_t::skip_t &skip) {
        assembler.get().increment_section_size(skip.size);
    }

    void first_pass::visit_ascii(stmt_t::ascii_t &ascii) {
        assembler.get().increment_section_size(ascii.value.size() + 1);
    }

    void first_pass::visit_equals(stmt_t::equals_t &) {
        throw std::logic_error("equals directive not implemented!");
    }

    void first_pass::visit_paramless_instr(stmt_t::instr_t::paramless_t &paramless) {
        if (paramless.type != mnemonic_t::IRET) {
            assembler.get().increment_section_size(4);
        } else {
            assembler.get().increment_section_size(8);
        }
    }

    void first_pass::visit_unary_instr(stmt_t::instr_t::unary_t &) {
        assembler.get().increment_section_size(4);
    }

    void first_pass::visit_binary_instr(stmt_t::instr_t::binary_t &) {
        assembler.get().increment_section_size(4);
    }

    void first_pass::visit_memory_instr(stmt_t::instr_t::memory_t &memory) {
        if (memory.type == mnemonic_t::LD) {
            switch (memory.operand.type) {
            case operand_t::type_t::REG_VALUE:
            case operand_t::type_t::REG_MEMORY_ADDR:
            case operand_t::type_t::REG_ADDR_LITERAL_OFFSET:
            case operand_t::type_t::REG_ADDR_SYMBOL_OFFSET:
                assembler.get().increment_section_size(4);
                break;
            case operand_t::type_t::LITERAL_VALUE:
            case operand_t::type_t::SYMBOL_VALUE:
                assembler.get().increment_section_size(8);
                break;
            case operand_t::type_t::LITERAL_ADDR:
            case operand_t::type_t::SYMBOL_ADDR:
                assembler.get().increment_section_size(12);
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
                assembler.get().increment_section_size(4);
                break;
            case operand_t::type_t::LITERAL_ADDR:
            case operand_t::type_t::SYMBOL_ADDR:
                assembler.get().increment_section_size(12);
                break;
            }
            return;
        }
        throw std::logic_error("invalid instruction type");
    }

    void first_pass::visit_uncond_instr(stmt_t::instr_t::uncond_t &) {
        assembler.get().increment_section_size(12);
    }

    void first_pass::visit_branch_instr(stmt_t::instr_t::branch_t &) {
        assembler.get().increment_section_size(12);
    }
} // m_asm::visitor
