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
    struct object_file_t;
}

namespace common::util::serde {
    void serialize(std::ofstream &ofs, symbol::object_file_t const &object_file);

    void deserialize(std::ifstream &ifs, symbol::object_file_t &object_file);
}

namespace common::symbol {
    struct object_file_t {
        symtab_t symtab;
        std::vector<section_t> sections;

        friend void util::serde::serialize(std::ofstream &ofs, object_file_t const &object_file);

        friend void util::serde::deserialize(std::ifstream &ifs, object_file_t &object_file);

        friend std::ostream &operator<<(std::ostream &os, object_file_t const &of);
    };

    std::ostream &operator<<(std::ostream &os, object_file_t const &of);
} // common

#endif //OBJECT_FILE_H
