//
// Created by djumi on 2/3/2024.
//

#ifndef OBJECT_FILE_H
#define OBJECT_FILE_H

#include <fstream>
#include <ostream>

#include "section_t.h"
#include "symtab_t.h"

namespace common::symbol {
    struct object_file {
        symtab_t symbtab;
        std::vector<section_t> sections;

        void serialize(std::ofstream &os) const;

        void deserialize(std::ifstream &is);

        friend std::ostream &operator<<(std::ostream &os, object_file const &of);
    };

    std::ostream &operator<<(std::ostream &os, object_file const &of);
} // common

#endif //OBJECT_FILE_H
