//
// Created by djumi on 1/30/2024.
//

#ifndef SECTION_T_H
#define SECTION_T_H

#include <cstdint>
#include <vector>
#include <string>

namespace m_asm::symbols {
    struct relocation_t {
        uint64_t offset;
        uint64_t symbol_index;
        uint64_t addend;

        relocation_t(const uint64_t offset, const uint64_t symbol_index,
                     const uint64_t addend)
            : offset(offset),
              symbol_index(symbol_index), addend(addend) {
        }
    };

    struct section_t {
        uint32_t index = s_index++;
        std::string name;
        uint32_t size = 0;
        std::vector<uint32_t> data;
        std::vector<relocation_t> relocations;

        explicit section_t(std::string name)
            : name(std::move(name)) {
        }

        static inline uint32_t s_index = 0;

        bool operator==(section_t const &other) const;

        bool operator!=(section_t const &other) const;
    };
} // m_asm::symbols

#endif //SECTION_T_H
