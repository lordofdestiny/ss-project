//
// Created by djumi on 2/3/2024.
//

// #include "common/object_file.h"

#include <iterator>
#include "../../include/common/object_file_t.h"

namespace common::symbol {
    void object_file_t::serialize(std::ofstream &os) const {
        symbtab.serialize(os);
        const auto section_size = sections.size();
        os.write(reinterpret_cast<const char *>(&section_size), sizeof(section_size));
        for (const auto &section: sections) {
            section.serialize(os);
        }
    }

    void object_file_t::deserialize(std::ifstream &is) {
        symbtab.deserialize(is);
        size_t sections_count = 0;
        is.read(reinterpret_cast<char *>(&sections_count), sizeof(sections_count));
        sections.resize(sections_count);
        for (auto &section: sections) {
            section.deserialize(is);
        }
    }

    std::ostream &operator<<(std::ostream &os, object_file_t const &of) {
        const auto &[symtab, sections] = of;
        os << of.symbtab;
        std::copy(sections.begin(), sections.end(),
                  std::ostream_iterator<section_t>(os, ""));
        return os;
    }
} // common
