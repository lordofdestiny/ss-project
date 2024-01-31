//
// Created by djumi on 11/22/2023.
//

#ifndef SS_PROJECT_STMT_T_H
#define SS_PROJECT_STMT_T_H

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include "utility.h"
#include "expr_t.h"

namespace m_asm::ast {
    namespace operand {
        struct reg_t {
            explicit reg_t(const uint8_t value = 0)
                : value(value < 16 ? value : value - 16),
                  is_csr(value >= 16) {
            }

            operator uint8_t() const {
                return get_ord();
            }

            [[nodiscard]] uint8_t get_ord() const {
                return is_csr ? value - 16 : value;
            }

            friend std::ostream &operator<<(std::ostream &os, reg_t reg);

            uint8_t value;
            bool is_csr;
        };

        std::ostream &operator<<(std::ostream &os, reg_t reg);

        using number_t = uint32_t;
        using symbol_t = std::string;
        using reg_literal_t = std::tuple<reg_t, number_t>;
        using reg_symbol_t = std::tuple<reg_t, symbol_t>;

        struct operand_t {
            enum class type_t {
                LITERAL_VALUE,
                SYMBOL_VALUE,
                LITERAL_ADDR,
                SYMBOL_ADDR,
                REG_VALUE,
                REG_MEMORY_ADDR,
                REG_ADDR_LITERAL_OFFSET,
                REG_ADDR_SYMBOL_OFFSET
            } type{};

            friend std::ostream &operator<<(std::ostream &os, operand_t &op);

            std::variant<
                reg_t, number_t, symbol_t,
                reg_literal_t, reg_symbol_t
            > value{};
        };

        std::ostream &operator<<(std::ostream &os, operand_t &op);
    };

    using word_argument_t = std::variant<std::string, uint32_t>;

    using word_arguments_t = std::vector<word_argument_t>;

    struct stmt_t {
        struct visitor_t;
        struct label_t;
        struct global_t;
        struct extern_t;
        struct section_t;
        struct word_t;
        struct skip_t;
        struct ascii_t;
        struct instr_t;
        struct equals_t;

        stmt_t() = default;

        stmt_t(stmt_t const &) = default;

        stmt_t(stmt_t &&) = default;

        stmt_t &operator=(stmt_t const &) = default;

        stmt_t &operator=(stmt_t &&) = default;

        virtual ~stmt_t() = default;

        virtual void accept(visitor_t &visitor) = 0;

        virtual void accept(visitor_t const &visitor) = 0;
    };

    struct stmt_t::instr_t : stmt_t {
        enum class mnemonic_t {
            HALT,
            INT, IRET,
            CALL, RET,
            JMP, BEQ, BNE, BGT,
            PUSH, POP,
            XCHG,
            ADD, SUB, MUL, DIV,
            NOT, AND, OR, XOR,
            SHL, SHR,
            LD, ST,
            CSRRD, CSRWR
        };

        struct paramless_t;
        struct unary_t;
        struct binary_t;
        struct memory_t;
        struct uncond_t;
        struct branch_t;

        explicit instr_t(mnemonic_t type)
            : type(type) {
        }

        mnemonic_t type;
    };


    extern std::array<std::string, 26> instruction_names;

    std::ostream &operator<<(std::ostream &os, stmt_t::instr_t::mnemonic_t t);

    struct stmt_t::visitor_t {
        visitor_t() = default;

        visitor_t(visitor_t const &) = default;

        visitor_t(visitor_t &&) = default;

        visitor_t &operator=(visitor_t const &) = default;

        visitor_t &operator=(visitor_t &&) = default;

        virtual ~visitor_t() = 0;

        virtual void visit_label(label_t &stmt) {
            const_cast<const visitor_t *>(this)->visit_label(stmt);
        }

        virtual void visit_label(label_t &) const {
        }

        virtual void visit_global(global_t &stmt) {
            const_cast<const visitor_t *>(this)->visit_global(stmt);
        }

        virtual void visit_global(global_t &) const {
        }

        virtual void visit_extern(extern_t &stmt) {
            const_cast<const visitor_t *>(this)->visit_extern(stmt);
        }

        virtual void visit_extern(extern_t &) const {
        }

        virtual void visit_section(section_t &stmt) {
            const_cast<const visitor_t *>(this)->visit_section(stmt);
        }

        virtual void visit_section(section_t &) const {
        }

        virtual void visit_word(word_t &stmt) {
            const_cast<const visitor_t *>(this)->visit_word(stmt);
        }

        virtual void visit_word(word_t &) const {
        }

        virtual void visit_skip(skip_t &stmt) {
            const_cast<const visitor_t *>(this)->visit_skip(stmt);
        }

        virtual void visit_skip(skip_t &) const {
        }

        virtual void visit_ascii(ascii_t &stmt) {
            const_cast<const visitor_t *>(this)->visit_ascii(stmt);
        }

        virtual void visit_ascii(ascii_t &) const {
        }

        virtual void visit_equals(equals_t &stmt) {
            const_cast<const visitor_t *>(this)->visit_equals(stmt);
        }

        virtual void visit_equals(equals_t &) const {
        }

        virtual void visit_paramless_instr(instr_t::paramless_t &instr) {
            const_cast<const visitor_t *>(this)->visit_paramless_instr(instr);
        }

        virtual void visit_paramless_instr(instr_t::paramless_t &) const {
        }

        virtual void visit_unary_instr(instr_t::unary_t &instr) {
            const_cast<const visitor_t *>(this)->visit_unary_instr(instr);
        }

        virtual void visit_unary_instr(instr_t::unary_t &) const {
        }

        virtual void visit_binary_instr(instr_t::binary_t &instr) {
            const_cast<const visitor_t *>(this)->visit_binary_instr(instr);
        }

        virtual void visit_binary_instr(instr_t::binary_t &) const {
        }

        virtual void visit_memory_instr(instr_t::memory_t &instr) {
            const_cast<const visitor_t *>(this)->visit_memory_instr(instr);
        }

        virtual void visit_memory_instr(instr_t::memory_t &) const {
        }

        virtual void visit_uncond_instr(instr_t::uncond_t &instr) {
            const_cast<const visitor_t *>(this)->visit_uncond_instr(instr);
        }

        virtual void visit_uncond_instr(instr_t::uncond_t &) const {
        }

        virtual void visit_branch_instr(instr_t::branch_t &instr) {
            const_cast<const visitor_t *>(this)->visit_branch_instr(instr);
        }

        virtual void visit_branch_instr(instr_t::branch_t &) const {
        }
    };

    struct stmt_t::label_t final : stmt_t {
        template<typename T,
            utility::enable_type_t<std::string, T>  = true>
        explicit label_t(T &&label): label(std::forward<T>(label)) {
        }

        void accept(visitor_t &visitor) final {
            visitor.visit_label(*this);
        }

        void accept(visitor_t const &visitor) final {
            visitor.visit_label(*this);
        }

        std::string label;
    };

    struct stmt_t::global_t final : stmt_t {
        template<typename T,
            utility::enable_type_t<utility::str_vec_t, T>  = true>
        explicit global_t(T &&symbols) : symbols(std::forward<T>(symbols)) {
        }

        void accept(visitor_t &visitor) final {
            visitor.visit_global(*this);
        }

        void accept(visitor_t const &visitor) final {
            visitor.visit_global(*this);
        }

        std::vector<std::string> symbols;
    };

    struct stmt_t::extern_t final : stmt_t {
        template<typename T,
            utility::enable_type_t<utility::str_vec_t, T>  = true>
        explicit extern_t(T &&symbols) : symbols(std::forward<T>(symbols)) {
        }

        void accept(visitor_t &visitor) final {
            visitor.visit_extern(*this);
        }

        void accept(visitor_t const &visitor) final {
            visitor.visit_extern(*this);
        }

        std::vector<std::string> symbols;
    };

    struct stmt_t::section_t final : stmt_t {
        template<typename T,
            utility::enable_type_t<std::string, T>  = true>
        explicit section_t(T &&name): name(std::forward<T>(name)) {
        }

        void accept(visitor_t &visitor) final {
            visitor.visit_section(*this);
        }

        void accept(visitor_t const &visitor) final {
            visitor.visit_section(*this);
        }

        std::string name;
    };

    struct stmt_t::word_t final : stmt_t {
        template<typename T,
            utility::enable_type_t<word_arguments_t, T>  = true>
        explicit word_t(T &&values) : word_values(std::forward<T>(values)) {
        }

        void accept(visitor_t &visitor) final {
            visitor.visit_word(*this);
        }

        void accept(visitor_t const &visitor) final {
            visitor.visit_word(*this);
        }

        word_arguments_t word_values;
    };

    struct stmt_t::skip_t final : stmt_t {
        explicit skip_t(uint32_t size) : size(size) {
        }

        void accept(visitor_t &visitor) final {
            visitor.visit_skip(*this);
        }

        void accept(visitor_t const &visitor) final {
            visitor.visit_skip(*this);
        }

        uint32_t size;
    };

    struct stmt_t::ascii_t final : stmt_t {
        template<typename T,
            utility::enable_type_t<std::string, T>  = true>
        explicit ascii_t(T &&value): value(std::forward<T>(value)) {
        }

        void accept(visitor_t &visitor) final {
            visitor.visit_ascii(*this);
        }

        void accept(visitor_t const &visitor) final {
            visitor.visit_ascii(*this);
        }

        std::string value;
    };


    struct stmt_t::equals_t final : stmt_t {
        template<typename T,
            utility::enable_type_t<std::string, T>  = true>
        equals_t(T &&symbol, std::unique_ptr<expr_t> expr)
            : symbol(std::forward<T>(symbol)),
              expr(std::move(expr)) {
        }

        void accept(visitor_t &visitor) final {
            visitor.visit_equals(*this);
        }

        void accept(visitor_t const &visitor) final {
            visitor.visit_equals(*this);
        }

        std::string symbol;
        std::unique_ptr<expr_t> expr;
    };

    struct stmt_t::instr_t::paramless_t final : instr_t {
        explicit paramless_t(const mnemonic_t type)
            : instr_t(type) {
        }

        void accept(visitor_t &visitor) override {
            visitor.visit_paramless_instr(*this);
        }

        void accept(visitor_t const &visitor) override {
            visitor.visit_paramless_instr(*this);
        }
    };

    struct stmt_t::instr_t::unary_t final : instr_t {
        unary_t(mnemonic_t type, operand::reg_t reg) : instr_t(type), reg(reg) {
        }

        void accept(visitor_t &visitor) override {
            visitor.visit_unary_instr(*this);
        }

        void accept(visitor_t const &visitor) override {
            visitor.visit_unary_instr(*this);
        }

        operand::reg_t reg;
    };

    struct stmt_t::instr_t::binary_t final : instr_t {
        binary_t(const mnemonic_t type,
                 const operand::reg_t first,
                 const operand::reg_t second)
            : instr_t(type), first(first), second(second) {
        }

        void accept(visitor_t &visitor) override {
            visitor.visit_binary_instr(*this);
        }

        void accept(visitor_t const &visitor) override {
            visitor.visit_binary_instr(*this);
        }

        operand::reg_t first, second;
    };

    struct stmt_t::instr_t::memory_t final : instr_t {
        template<typename T,
            utility::enable_type_t<operand::operand_t, T>  = true>
        memory_t(const mnemonic_t type,
                 const operand::reg_t reg, T &&operand)
            : instr_t(type), reg(reg),
              operand(std::forward<T>(operand)) {
        }

        template<typename T,
            utility::enable_type_t<operand::operand_t, T>  = true>
        memory_t(const mnemonic_t type, T &&operand,
                 const operand::reg_t reg)
            : instr_t(type), reg(reg),
              operand(std::forward<T>(operand)) {
        }

        void accept(visitor_t &visitor) override {
            visitor.visit_memory_instr(*this);
        }

        void accept(visitor_t const &visitor) override {
            visitor.visit_memory_instr(*this);
        }

        operand::reg_t reg;
        operand::operand_t operand;
    };

    struct stmt_t::instr_t::uncond_t final : instr_t {
        template<typename T,
            utility::enable_type_t<operand::operand_t, T>  = true>
        uncond_t(const mnemonic_t type, T &&operand)
            : instr_t(type), operand(std::forward<T>(operand)) {
        }

        void accept(visitor_t &visitor) override {
            visitor.visit_uncond_instr(*this);
        }

        void accept(visitor_t const &visitor) override {
            visitor.visit_uncond_instr(*this);
        }

        operand::operand_t operand;
    };

    struct stmt_t::instr_t::branch_t final : instr_t {
        template<typename T,
            utility::enable_type_t<operand::operand_t, T>  = true>
        branch_t(const mnemonic_t type,
                 const int reg1, const int reg2,
                 T &&operand)
            : instr_t(type),
              reg1(reg1), reg2(reg2),
              operand(std::forward<T>(operand)) {
        }

        void accept(visitor_t &visitor) override {
            visitor.visit_branch_instr(*this);
        }

        void accept(visitor_t const &visitor) override {
            visitor.visit_branch_instr(*this);
        }

        operand::reg_t reg1, reg2;
        operand::operand_t operand;
    };
} // ast

#endif //SS_PROJECT_STMT_T_H
