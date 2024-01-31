//
// Created by djumi on 1/30/2024.
//

#ifndef SS_ALE_SYMBOL_T_H
#define SS_ALE_SYMBOL_T_H

#include <string>
#include <utility>
#include <variant>
#include <cstdint>

namespace m_asm::symbols {
    struct symbol_t {
        enum class type_t {
            SECTION, NOTYPE
        };

        uint32_t index = s_index++;
        std::string name;
        uint32_t section_index;
        type_t type;
        uint32_t value;
        char local;
        bool is_equ = false;

        symbol_t(std::string name, const uint32_t section_index,const uint32_t value,
                const char local = ' ', const type_t type = type_t::NOTYPE,const bool is_equ = false)
                : name(std::move(name)), section_index(section_index), type(type),
                  value(value), local(local), is_equ(is_equ) {}


        friend std::ostream &operator<<(std::ostream &os, symbol_t const &symbol);

    private:
        static inline uint32_t s_index = 0;
    };

    std::ostream &operator<<(std::ostream &os, symbol_t const &symbol);
}


#endif //SS_ALE_SYMBOL_T_H
