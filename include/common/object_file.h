//
// Created by djumi on 2/3/2024.
//

#ifndef OBJECT_FILE_H
#define OBJECT_FILE_H

#include "section_t.h"
#include "symtab_t.h"

namespace common::symbol {
    struct object_file {
        symtab_t symbtab;
        std::vector<section_t> sections;
    };
} // common

#endif //OBJECT_FILE_H
