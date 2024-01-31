//
// Created by djumi on 1/26/2024.
//

#ifndef SS_PROJECT_PRINTER_H
#define SS_PROJECT_PRINTER_H

#include <iostream>
#include <sstream>

#include "parser_driver.h"
#include "ast/stmt_t.h"

namespace m_asm::visitor {
    class source_printer final : public ast::stmt_t::visitor_t,
                                 public ast::expr_t::visitor_t {
    protected:
        std::stringstream ss;

    public:
        source_printer() = default;

        source_printer(source_printer const &) = delete;

        source_printer(source_printer &&) noexcept = default;

        ~source_printer() override = default;

        void visit(parsed_file_t const &file) {
            for (const auto &statement: file) {
                statement->accept(*this);
                ss << '\n';
            }
        }

        void clear() {
            ss.str(std::string());
        }

        std::string to_string() const {
            return ss.str();
        }

    private:
        void print_word_arg(ast::word_argument_t const &wordArg);

        void visit_label(ast::stmt_t::label_t &label) override;

        void visit_global(ast::stmt_t::global_t &global) override;

        void visit_extern(ast::stmt_t::extern_t &anExtern) override;

        void visit_section(ast::stmt_t::section_t &section) override;

        void visit_word(ast::stmt_t::word_t &word) override;

        void visit_skip(ast::stmt_t::skip_t &skip) override;

        void visit_ascii(ast::stmt_t::ascii_t &ascii) override;

        void visit_equals(ast::stmt_t::equals_t &equals) override;

        void visit_paramless_instr(ast::stmt_t::instr_t::paramless_t &paramless) override;

        void visit_unary_instr(ast::stmt_t::instr_t::unary_t &unary) override;

        void visit_binary_instr(ast::stmt_t::instr_t::binary_t &binary) override;

        void visit_memory_instr(ast::stmt_t::instr_t::memory_t &memory) override;

        void visit_uncond_instr(ast::stmt_t::instr_t::uncond_t &uncond) override;

        void visit_branch_instr(ast::stmt_t::instr_t::branch_t &branch) override;

        void visit_int_literal(ast::expr_t::int_literal_t &literal) override;

        void visit_symbol(ast::expr_t::symbol_t &symbol) override;

        void visit_unary(ast::expr_t::unary_t &unary) override;

        void visit_sum(ast::expr_t::sum_t &sum) override;
    };
}

#endif //SS_PROJECT_PRINTER_H
