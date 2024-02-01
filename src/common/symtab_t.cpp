//
// Created by djumi on 1/30/2024.
//

#include <ostream>
#include <iomanip>
#include <algorithm>
#include "common/symtab_t.h"

namespace m_asm::symbols {
    auto symtab_t::find(std::string const &symbol_name) -> symbol_t * {
        return const_cast<symbol_t *>(const_cast<const symtab_t *>(this)->find(symbol_name));
    }

    auto symtab_t::find(std::string const &symbol_name) const -> symbol_t const * {
        if (const auto it = std::find_if(
            begin(), end(),
            [&](const auto &symbol) {
                return symbol.name == symbol_name;
            }); it != end()) {
            return &*it;
        }

        return nullptr;
    }

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
