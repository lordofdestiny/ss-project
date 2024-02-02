//
// Created by djumi on 1/31/2024.
//

#include <exception>
#include "visitors/second_pass.h"
#include "ast/stmt_t.h"

namespace m_asm::visitor {
    void second_pass::visit_global(stmt_t::global_t &global) {
        auto symtab = asm_ref.get().get_symbol_table();
        for (const auto &symbol_name: global.symbols) {
            if (const auto symbol = symtab.find(symbol_name); symbol == nullptr) {
                throw std::logic_error("symbol " + symbol_name + " not found");
            } else {
                symbol->local = 'g';
            }
        }
    }

    void second_pass::visit_section(stmt_t::section_t &section) {
        asm_ref.get().set_current_section(section.name);
    }

    void second_pass::visit_word(stmt_t::word_t &word) {
        for (const auto &value: word.word_values) {
            if (const auto value_ptr = std::get_if<uint32_t>(&value)) {
                asm_ref.get().write_word(*value_ptr);
                continue;
            }

            // Else it holds a string - symbol name
            const auto &symbol_name = std::get<std::string>(value);
            const auto &symtab = asm_ref.get().get_symbol_table();
            const auto symbol_it = std::find_if(
                symtab.begin(), symtab.end(), [&](const auto &symbol) {
                    return symbol.name == symbol_name;
                });
            if (symbol_it == symtab.end()) {
                throw std::logic_error("symbol '" + symbol_name + "' does not exit");
            }
            asm_ref.get().add_relocation(
                asm_ref.get().get_section_position() + 4,
                symbol_it->local
                    ? symbol_it->section_index
                    : std::distance(symtab.begin(), symbol_it),
                symbol_it->local ? symbol_it->value : 0
            );

            asm_ref.get().write_word(0);
        }
    }

    void second_pass::visit_skip(stmt_t::skip_t &skip) {
        asm_ref.get().write_zeros(skip.size);
    }

    void second_pass::visit_ascii(stmt_t::ascii_t &ascii) {
        asm_ref.get().write_string(ascii.value);
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
