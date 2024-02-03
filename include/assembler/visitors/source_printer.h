//
// Created by djumi on 1/26/2024.
//

#ifndef SS_PROJECT_PRINTER_H
#define SS_PROJECT_PRINTER_H

#include <iostream>
#include <sstream>


#include "parser_driver.h"
#include "common/operand.h"
#include "ast/stmt_t.h"

namespace m_asm::visitor {
    class source_printer final : public stmt_t::visitor_t,
                                 public expr_t::visitor_t {
    protected:
        std::stringstream ss;

    public:
        source_printer() = default;

        source_printer(source_printer const &) = delete;

        source_printer(source_printer &&) noexcept = default;

        ~source_printer() override = default;

        void visit(parsed_file_t const &file) {
            for (const auto &statement: file) {
                visit(*statement);
                ss << '\n';
            }
        }

        void visit(stmt_t &stmt) {
            stmt.accept(*this);
        }

        void clear() {
            ss.str(std::string());
        }

        std::string to_string() const {
            return ss.str();
        }

    private:
        void print_word_arg(common::word_argument_t const &wordArg);

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

        void visit_int_literal(expr_t::int_literal_t &literal) override;

        void visit_symbol(expr_t::symbol_t &symbol) override;

        void visit_unary(expr_t::unary_t &unary) override;

        void visit_sum(expr_t::sum_t &sum) override;
    };
}

#endif //SS_PROJECT_PRINTER_H
