//
// Created by djumi on 2/1/2024.
//

#include <ostream>

#include "common/relocation_t.h"
#include "common/util.h"

namespace common::symbol {
    void relocation_t::serialize(std::ofstream &os) const {
        os.write(reinterpret_cast<const char *>(&offset), sizeof(offset));
        os.write(reinterpret_cast<const char *>(&symbol), sizeof(symbol));
        os.write(reinterpret_cast<const char *>(&addend), sizeof(addend));
    }

    void relocation_t::deserialize(std::ifstream &is) {
        is.read(reinterpret_cast<char *>(&offset), sizeof(offset));
        is.read(reinterpret_cast<char *>(&symbol), sizeof(symbol));
        is.read(reinterpret_cast<char *>(&addend), sizeof(addend));
    }

    std::ostream &operator<<(std::ostream &os, relocation_t const &reloc) {
        using util::hex_to_string;
        auto &&[offset, symbol, addend] = reloc;
        return os << "[" << hex_to_string(offset) << "]" << " = {" << symbol << "} + " << addend;
    }
} // common
