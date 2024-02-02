//
// Created by djumi on 1/30/2024.
//

#include "common/section_t.h"

namespace m_asm::symbols {
    bool section_t::operator==(const section_t &other) const {
        return name == other.name;
    }

    bool section_t::operator!=(const section_t &other) const {
        return !(*this == other);
    }

    std::ostream &operator<<(std::ostream &os, const section_t &section) {
        auto &&[index, name, size, data, relocations] = section;
        os << "(" << index << ")[" << name << "]";
        os << "{size = " << size << ", data_size = " << data.size() << "}";
        os << "<relocations = " << relocations.size() << ">";
        return os;
    }
} // m_asm::symbols
