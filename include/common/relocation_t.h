//
// Created by djumi on 2/1/2024.
//

#ifndef RELOCATION_H
#define RELOCATION_H
#include <cstdint>
#include <ostream>

namespace common {
    struct relocation_t {
        uint64_t offset;
        uint64_t symbol;
        uint64_t addend;

        relocation_t(const uint64_t offset, const uint64_t symbol, const uint64_t addend)
            : offset(offset), symbol(symbol), addend(addend) {
        }

        friend std::ostream &operator<<(std::ostream &os, relocation_t const &reloc);
    };

    std::ostream &operator<<(std::ostream &os, relocation_t const &reloc);
} // common

#endif //RELOCATION_H
