//
// Created by djumi on 2/4/2024.
//

#ifndef SERDE_H
#define SERDE_H

#include <fstream>

#include  "symtab_t.h"
#include "symbol_t.h"
#include "section_t.h"
#include "relocation_t.h"
#include "object_file_t.h"
#include "exec_file_t.h"

namespace common::util::serde {
    void serialize(std::ofstream &ofs, symbol::object_file_t const &object_file);

    void deserialize(std::ifstream &ifs, symbol::object_file_t &object_file);

    void serialize(std::ofstream &ofs, symbol::exec_file_t const &object_file);

    void deserialize(std::ifstream &ifs, symbol::exec_file_t &object_file);

    void serialize(std::ofstream &ofs, symbol::symbol_t const &symbol);

    void deserialize(std::ifstream &ifs, symbol::symbol_t &symbol);

    void serialize(std::ofstream &ofs, symbol::symtab_t const &symtab);

    void deserialize(std::ifstream &ifs, symbol::symtab_t &symtab);

    void serialize(std::ofstream &ofs, symbol::section_t const &section);

    void deserialize(std::ifstream &ifs, symbol::section_t &section);

    void serialize(std::ofstream &ofs, symbol::relocation_t const &relocation);

    void deserialize(std::ifstream &ifs, symbol::relocation_t &relocation);
}

#endif //SERDE_H
