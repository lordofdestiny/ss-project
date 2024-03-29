//
// Created by djumi on 1/30/2024.
//

#ifndef SECTION_T_H
#define SECTION_T_H

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <ostream>
#include "relocation_t.h"


namespace common::symbol {
    struct section_t;
}

namespace common::util::serde {
    void serialize(std::ofstream &ofs, symbol::section_t const &section);

    void deserialize(std::ifstream &ifs, symbol::section_t &section);
}

namespace common::symbol {
    struct section_t {
        static constexpr size_t SECTION_ABS = static_cast<size_t>(-2);
        static constexpr size_t SECTION_UNDEF = static_cast<size_t>(-1);

        uint32_t index;
        std::string name;
        uint32_t size = 0;
        std::vector<uint8_t> data;
        std::vector<relocation_t> relocations;

        section_t() = default;

        explicit section_t(const size_t index, std::string name)
            : index(index), name(std::move(name)) {
        }

        static inline uint32_t s_index = 0;

        bool operator==(section_t const &other) const;

        bool operator!=(section_t const &other) const;

        friend void util::serde::serialize(std::ofstream &ofs, section_t const &section);

        friend void util::serde::deserialize(std::ifstream &ifs, section_t &section);

        friend std::ostream &operator<<(std::ostream &os, const section_t &section);
    };

    std::ostream &operator<<(std::ostream &os, const section_t &section);
} // m_asm::symbols

#endif //SECTION_T_H
