//
// Created by djumi on 1/30/2024.
//


#include <ostream>
#include <iomanip>

#include "symbols/symbol_t.h"
#include "utility.h"

namespace m_asm::symbols {

    std::ostream &operator<<(std::ostream &os, symbol_t const &symbol) {
        using utility::hex_to_string;
        os << std::setw(4) << std::dec << symbol.index;
        os << std::setw(7) << (symbol.type == symbol_t::type_t::SECTION ? "SCTN" : "NOTYPE");
        os << std::setw(7)
           << (symbol.section_index != -1u ? std::to_string(symbol.section_index) : "*ABS*");
        os << std::hex << std::setw(10) << hex_to_string(symbol.value);
        os << "  " << symbol.local << "  ";
        os << (symbol.name.empty() ? "*UND*" : symbol.name);
        return os;
    }
}