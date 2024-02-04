//
// Created by djumi on 2/3/2024.
//

// #include "common/object_file.h"

#include <iterator>
#include "../../include/common/object_file_t.h"

namespace common::symbol {
    std::ostream &operator<<(std::ostream &os, object_file_t const &of) {
        const auto &[symtab, sections] = of;
        os << of.symtab;
        std::copy(sections.begin(), sections.end(),
                  std::ostream_iterator<section_t>(os, ""));
        return os;
    }
} // common
