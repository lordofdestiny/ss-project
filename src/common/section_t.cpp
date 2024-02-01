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
} // m_asm::symbols
