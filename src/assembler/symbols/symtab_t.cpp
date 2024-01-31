//
// Created by djumi on 1/30/2024.
//

#include <ostream>
#include <iomanip>
#include "symbols/symtab_t.h"

namespace m_asm::symbols {

    std::ostream &operator<<(std::ostream &os, const symtab_t &table) {
        os << std::setw(4) << "id";
        os << std::setw(7) << "type";
        os << std::setw(7) << "Ndx";
        os << std::setw(10) << "value";
        os << "  " << "V" << "  " << "name\n";
        for (auto const &symbol: table) {
            os << symbol << std::endl;
        }
        return os;
    }
}
