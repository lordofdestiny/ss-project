//
// Created by djumi on 1/26/2024.
//

#include <iterator>
#include <variant>
#include "ast/stmt_t.h"
#include "utility.h"
#include "visitors/source_printer.h"

namespace m_asm::visitor {
    void source_printer::visit_label(ast::stmt_t::label_t &label) {
        ss << label.label << ":";
    }

    void source_printer::visit_global(ast::stmt_t::global_t &global) {
        ss << "GLOBAL" << " ";
        for (size_t i = 0; i < global.symbols.size() - 1; i++) {
            ss << global.symbols[i] << ", ";
        }
        ss << global.symbols.back() << ";";
    }

    void source_printer::visit_extern(ast::stmt_t::extern_t &anExtern) {
        ss << "EXTERN" << " ";
        for (size_t i = 0; i < anExtern.symbols.size() - 1; i++) {
            ss << anExtern.symbols[i] << ", ";
        }
        ss << anExtern.symbols.back();
    }

    void source_printer::visit_section(ast::stmt_t::section_t &section) {
        ss << "SECTION " << section.name;
    }

    void source_printer::print_word_arg(ast::word_argument_t const &wordArg) {
        std::visit([&](auto &&x) { ss << x; }, wordArg);
    }

    void source_printer::visit_word(ast::stmt_t::word_t &word) {
        ss << "WORD ";
        for (size_t i = 0; i < word.word_values.size() - 1; i++) {
            print_word_arg(word.word_values[i]);
            ss << ", ";
        }
        print_word_arg(word.word_values.back());
    }

    void source_printer::visit_equals(ast::stmt_t::equals_t &equals) {
        ss << "EQU ";
        equals.expr->accept(*this);
    }

    void source_printer::visit_skip(ast::stmt_t::skip_t &skip) {
        ss << "SKIP " << skip.size;
    }

    void source_printer::visit_ascii(ast::stmt_t::ascii_t &ascii) {
        ss << "ASCII " << ascii.value;
    }

    void source_printer::visit_paramless_instr(ast::stmt_t::instr_t::paramless_t &paramless) {
        ss << paramless.type;
    }

    void source_printer::visit_unary_instr(ast::stmt_t::instr_t::unary_t &unary) {
        ss << unary.type << " " << unary.reg;
    }

    void source_printer::visit_binary_instr(ast::stmt_t::instr_t::binary_t &binary) {
        ss << binary.type << " " << binary.first << ", " << binary.second;
    }

    void source_printer::visit_memory_instr(ast::stmt_t::instr_t::memory_t &memory) {
        ss << memory.type << " ";
        if (memory.type == stmt_t::instr_t::mnemonic_t::LD) {
            ss << memory.operand << ", " << memory.reg;
        } else if (memory.type == stmt_t::instr_t::mnemonic_t::ST) {
            ss << memory.reg << ", " << memory.operand;
        }
    }

    void source_printer::visit_uncond_instr(ast::stmt_t::instr_t::uncond_t &uncond) {
        ss << uncond.type << " " << uncond.operand;
    }

    void source_printer::visit_branch_instr([[maybe_unused]] ast::stmt_t::instr_t::branch_t &branch) {
        ss << branch.type << " " << branch.reg1 << ", " << branch.reg2 << ", " << branch.operand;
    }

    void source_printer::visit_int_literal(ast::expr_t::int_literal_t &literal) {
        ss << literal.value;
    }

    void source_printer::visit_symbol(ast::expr_t::symbol_t &symbol) {
        ss << symbol.name;
    }

    void source_printer::visit_unary(ast::expr_t::unary_t &unary) {
        ss << "(" << unary.sign;
        unary.value->accept(*this);
        ss << ")";
    }

    void source_printer::visit_sum(ast::expr_t::sum_t &sum) {
        sum.left->accept(*this);
        ss << " " << sum.op << " ";
        sum.right->accept(*this);
    }
}
