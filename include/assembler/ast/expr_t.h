//
// Created by djumi on 11/23/2023.
//

#ifndef SS_PROJECT_EXPR_T_H
#define SS_PROJECT_EXPR_T_H

#include <cstdint>
#include <string>
#include <memory>

#include "common/util.h"

namespace m_asm::ast {
    struct expr_t {
        struct visitor_t;
        struct int_literal_t;
        struct symbol_t;
        struct unary_t;
        struct sum_t;

        expr_t() = default;

        expr_t(expr_t const &) = default;

        expr_t(expr_t &&) = default;

        expr_t &operator=(expr_t const &) = default;

        expr_t &operator=(expr_t &&) = default;

        virtual ~expr_t() = default;

        virtual void accept(visitor_t &visitor) = 0;

        virtual void accept(visitor_t const &visitor) = 0;
    };

    struct expr_t::visitor_t {
        visitor_t() = default;

        visitor_t(visitor_t const &) = default;

        visitor_t(visitor_t &&) = default;

        visitor_t &operator=(visitor_t const &) = default;

        visitor_t &operator=(visitor_t &&) = default;

        virtual ~visitor_t() = 0;

        virtual void visit_int_literal(int_literal_t &op) {
            const_cast<const visitor_t *>(this)->visit_int_literal(op);
        }

        virtual void visit_int_literal(int_literal_t &) const {
        }

        virtual void visit_symbol(symbol_t &op) {
            const_cast<const visitor_t *>(this)->visit_symbol(op);
        }

        virtual void visit_symbol(symbol_t &) const {
        }

        virtual void visit_unary(unary_t &op) {
            const_cast<const visitor_t *>(this)->visit_unary(op);
        }

        virtual void visit_unary(unary_t &) const {
        }

        virtual void visit_sum(sum_t &op) {
            const_cast<const visitor_t *>(this)->visit_sum(op);
        }

        virtual void visit_sum(sum_t &) const {
        }
    };

    struct expr_t::int_literal_t final : expr_t {
        explicit int_literal_t(const uint32_t value) : value(value) {
        }

        void accept(visitor_t &visitor) override {
            visitor.visit_int_literal(*this);
        };

        void accept(visitor_t const &visitor) override {
            visitor.visit_int_literal(*this);
        }

        uint32_t value;
    };

    struct expr_t::symbol_t final : expr_t {
        template<typename T,
            common::util::enable_type_t<std::string, T>  = true>
        explicit symbol_t(T &&name) : name(name) {
        }

        void accept(visitor_t &visitor) override {
            visitor.visit_symbol(*this);
        };

        void accept(visitor_t const &visitor) override {
            visitor.visit_symbol(*this);
        }

        std::string name;
    };

    struct expr_t::unary_t final : expr_t {
        explicit unary_t(const char sign, std::unique_ptr<expr_t> value)
            : sign(sign), value(std::move(value)) {
        }

        void accept(visitor_t &visitor) override {
            visitor.visit_unary(*this);
        };

        void accept(visitor_t const &visitor) override {
            visitor.visit_unary(*this);
        }

        char sign;
        std::unique_ptr<expr_t> value;
    };

    struct expr_t::sum_t final : expr_t {
        explicit sum_t(std::unique_ptr<expr_t> left, const char op,
                       std::unique_ptr<expr_t> right)
            : left(std::move(left)), op(op),
              right(std::move(right)) {
        }

        void accept(visitor_t &visitor) override {
            visitor.visit_sum(*this);
        };

        void accept(visitor_t const &visitor) override {
            visitor.visit_sum(*this);
        }

        std::unique_ptr<expr_t> left;
        char op;
        std::unique_ptr<expr_t> right;
    };
}

#endif //SS_PROJECT_EXPR_T_H
