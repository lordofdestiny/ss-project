//
// Created by djumi on 1/30/2024.
//


#include <ostream>
#include <iomanip>
#include <string>

#include "common/symbol_t.h"
#include "common/util.h"

namespace common::symbol {
    void symbol_t::serialize(std::ofstream &os) const {
        os.write(reinterpret_cast<const char *>(&index), sizeof(index));
        os.write(name.data(), name.size() + 1);
        os.write(reinterpret_cast<const char *>(&section_index), sizeof(section_index));
        os.write(reinterpret_cast<const char *>(&type), sizeof(type));
        os.write(reinterpret_cast<const char *>(&value), sizeof(value));
        os.write(&local, sizeof(local));
        os.write(reinterpret_cast<const char *>(&is_equ), sizeof(is_equ));
        os.write(reinterpret_cast<const char *>(&has_value), sizeof(has_value));
    }

    void symbol_t::deserialize(std::ifstream &is) {
        is.read(reinterpret_cast<char *>(&index), sizeof(index));
        std::getline(is, name, '\0');
        is.read(reinterpret_cast<char *>(&section_index), sizeof(section_index));
        is.read(reinterpret_cast<char *>(&type), sizeof(type));
        is.read(reinterpret_cast<char *>(&value), sizeof(value));
        is.read(&local, sizeof(local));
        is.read(reinterpret_cast<char *>(&is_equ), sizeof(is_equ));
        is.read(reinterpret_cast<char *>(&has_value), sizeof(has_value));
    }


    std::ostream &operator<<(std::ostream &os, symbol_t const &symbol) {
        using namespace std::string_literals;
        using util::hex_to_string;

        os << std::setw(4) << std::dec << symbol.index;
        os << std::setw(7) << (symbol.type == symbol_t::type_t::SECTION ? "SCTN" : "NOTYPE");
        os << std::setw(7)
                << (symbol.section_index != -1u ? std::to_string(symbol.section_index) : "*ABS*"s);
        os << std::hex << std::setw(10) << hex_to_string(symbol.value);
        os << "  " << symbol.local << "  ";
        os << (symbol.name.empty() ? "*UND*"s : symbol.name);
        return os << std::dec;
    }
}
