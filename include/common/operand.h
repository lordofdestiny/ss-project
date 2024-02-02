//
// Created by djumi on 2/2/2024.
//

#ifndef OPERAND_H
#define OPERAND_H

#include <cstdint>
#include <ostream>
#include <variant>
#include <vector>
#include <tuple>

namespace common {
    namespace operand {
        struct reg_t {
            reg_t(const uint8_t value = 0)
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

            friend std::ostream &operator<<(std::ostream &os, operand_t const &op);

            std::variant<
                reg_t, number_t, symbol_t,
                reg_literal_t, reg_symbol_t
            > value{};
        };

        std::ostream &operator<<(std::ostream &os, operand_t const &op);
    }

    using word_argument_t = std::variant<std::string, uint32_t>;

    using word_arguments_t = std::vector<word_argument_t>;
}

#endif //OPERAND_H
