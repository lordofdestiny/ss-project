//
// Created by djumi on 2/1/2024.
//

#include <ostream>

#include "common/relocation_t.h"
#include "common/util.h"

namespace common::symbol {
    std::ostream &operator<<(std::ostream &os, relocation_t const &reloc) {
        using util::hex_to_string;
        auto &&[offset, symbol, addend] = reloc;
        return os << "[" << hex_to_string(offset) << "]" << " = {" << symbol << "} + " << addend;
    }
} // common
