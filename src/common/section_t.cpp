//
// Created by djumi on 1/30/2024.
//

#include "common/section_t.h"

#include <iomanip>
#include <iterator>

#include "common/util.h"

namespace common::symbol {
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
        os << "<relocations = " << relocations.size() << ">\n";
        if (!section.data.empty()) {
            os << std::string(25, '-') << " CONTENTS " << std::string(25, '-') << '\n';
            // Header
            os << std::string(12, ' ');
            os << std::setfill('0') << std::hex;
            for (int i = 0; i < 16; i++) {
                os << std::setw(2) << i << " ";
            }
            os << '\n';
            for (size_t i = 0; i < section.size; i += 16) {
                os << "0x" << std::setw(8) << i << ": ";
                for (size_t j = 0; j < 16 && i + j < section.size; j++) {
                    os << std::setw(2) << +section.data[i + j] << " ";
                }
                os << '\n';
            }
        }
        if (!relocations.empty()) {
            std::copy(relocations.begin(), relocations.end(),
                      std::ostream_iterator<relocation_t>(os, "\n"));
        }
        return os << std::dec << std::setfill(' ');
    }
} // m_asm::symbols
