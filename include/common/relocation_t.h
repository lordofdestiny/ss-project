//
// Created by djumi on 2/1/2024.
//

#ifndef RELOCATION_H
#define RELOCATION_H
#include <cstdint>
#include <fstream>
#include <ostream>

namespace common::symbol {
    struct relocation_t;
}

namespace common::util::serde {
    void serialize(std::ofstream &ofs, symbol::relocation_t const &relocation);

    void deserialize(std::ifstream &ifs, symbol::relocation_t &relocation);
}

namespace common::symbol {
    struct relocation_t {
        uint64_t offset;
        uint64_t symbol;
        uint64_t addend;


        explicit relocation_t(const uint64_t offset = 0,
                              const uint64_t symbol = 0,
                              const uint64_t addend = 0)
            : offset(offset), symbol(symbol), addend(addend) {
        }

        friend void util::serde::serialize(std::ofstream &ofs, relocation_t const &relocation);

        friend void util::serde::deserialize(std::ifstream &ifs, relocation_t &relocation);

        friend std::ostream &operator<<(std::ostream &os, relocation_t const &reloc);
    };

    std::ostream &operator<<(std::ostream &os, relocation_t const &reloc);
} // common

#endif //RELOCATION_H
