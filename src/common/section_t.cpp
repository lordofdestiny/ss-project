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

    void section_t::serialize(std::ofstream &os) const {
        os.write(reinterpret_cast<const char *>(&index), sizeof(index));
        os.write(name.data(), name.size() + 1);
        os.write(reinterpret_cast<const char *>(&size), sizeof(size));

        os.write(reinterpret_cast<const char *>(data.data()), data.size());

        const auto &relocation_count = relocations.size();
        os.write(reinterpret_cast<const char *>(&relocation_count), sizeof(relocation_count));
        for (const auto &relocation: relocations) {
            relocation.serialize(os);
        }
    }

    void section_t::deserialize(std::ifstream &is) {
        is.read(reinterpret_cast<char *>(&index), sizeof(index));
        std::getline(is, name, '\0');
        is.read(reinterpret_cast<char *>(&size), sizeof(size));

        size_t content_size = 0;
        is.read(reinterpret_cast<char *>(&content_size), sizeof(content_size));
        data.resize(content_size);
        is.read(reinterpret_cast<char *>(data.data()), content_size);

        size_t relocation_count = 0;
        is.read(reinterpret_cast<char *>(&relocation_count), sizeof(relocation_count));
        relocations.resize(relocation_count);
        for (auto &relocation: relocations) {
            relocation.deserialize(is);
        }
    }

    std::ostream &operator<<(std::ostream &os, const section_t &section) {
        auto &&[index, name, size, data, relocations] = section;
        os << "(" << index << ")[" << name << "]";
        os << "{size = " << size << ", data_size = " << data.size() << "}";
        os << "<relocations = " << relocations.size() << ">\n";
        if (section.data.size() > 0) {
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
        if (relocations.size() > 0) {
            std::copy(relocations.begin(), relocations.end(),
                      std::ostream_iterator<relocation_t>(os, "\n"));
        }
        return os << std::dec << std::setfill(' ');
    }
} // m_asm::symbols
