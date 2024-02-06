//
// Created by djumi on 1/30/2024.
//


#include <ostream>
#include <iomanip>
#include <string>

#include "common/symbol_t.h"

#include "common/section_t.h"
#include "common/util.h"

namespace common::symbol {
    std::ostream &operator<<(std::ostream &os, symbol_t const &symbol) {
        using namespace std::string_literals;
        using util::hex_to_string;

        os << std::setw(4) << std::dec << symbol.index;
        os << std::setw(7) << (symbol.type == symbol_t::type_t::SECTION ? "SCTN" : "NOTYPE");
        os << std::setw(7) << (symbol.section_index == (section_t::SECTION_UNDEF & 0xFF'FF'FF'FF)
                                   ? "*UND*"s
                                   : std::to_string(symbol.section_index));
        os << std::hex << std::setw(10) << hex_to_string(symbol.value);
        os << "  " << symbol.local << "  ";
        os << symbol.name;
        return os << std::dec;
    }
}
