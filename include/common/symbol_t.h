//
// Created by djumi on 1/30/2024.
//

#ifndef SS_ALE_SYMBOL_T_H
#define SS_ALE_SYMBOL_T_H

#include <string>
#include <utility>
#include <variant>
#include <cstdint>
#include <ostream>
#include <fstream>


namespace common::symbol {
    struct symbol_t;
}

namespace common::util::serde {
    void serialize(std::ofstream &ofs, symbol::symbol_t const &symbol);

    void deserialize(std::ifstream &ifs, symbol::symbol_t &symbol);
}

namespace common::symbol {
    struct symbol_t {
        enum class type_t {
            SECTION, NOTYPE, UNSET
        };

        uint32_t index = s_index++;
        std::string name;
        uint32_t section_index;
        type_t type;
        uint32_t value;
        char local;
        bool is_equ;
        bool has_value;

        symbol_t() = default;

        symbol_t(std::string name, const uint32_t section_index, const uint32_t value,
                 const bool has_value = false, const char local = ' ',
                 const type_t type = type_t::NOTYPE, const bool is_equ = false)
            : name(std::move(name)), section_index(section_index), type(type),
              value(value), local(local), is_equ(is_equ), has_value(has_value) {
        }

        symbol_t(symbol_t const &other)
            : name(other.name), section_index(other.section_index),
              type(other.type), value(other.value), local(other.local),
              is_equ(other.is_equ), has_value(other.has_value) {
        }

        symbol_t(symbol_t &&other) noexcept
            : index(std::exchange(other.index, 0)),
              name(std::move(other.name)),
              section_index(std::exchange(other.section_index, 0)),
              type(std::exchange(other.type, type_t::UNSET)),
              value(std::exchange(other.value, 0)),
              local(std::exchange(other.local, ' ')),
              is_equ(std::exchange(other.is_equ, false)),
              has_value(std::exchange(other.has_value, false)) {
        }

        friend void util::serde::serialize(std::ofstream &ofs, symbol_t const &symbol);

        friend void util::serde::deserialize(std::ifstream &ifs, symbol_t &symbol);

        friend std::ostream &operator<<(std::ostream &os, symbol_t const &symbol);

        static inline uint32_t s_index = 0;
    };

    std::ostream &operator<<(std::ostream &os, symbol_t const &symbol);
}


#endif //SS_ALE_SYMBOL_T_H
