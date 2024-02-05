//
// Created by djumi on 2/5/2024.
//

#ifndef LINKER_H
#define LINKER_H

#include <unordered_map>

#include "../common/symtab_t.h"
#include "../common/object_file_t.h"
#include "../common/operand.h"
#include "../common/util.h"

namespace m_lnk {
    class linker {
    public:
        [[nodiscard]] common::symbol::object_file_t
        link(std::vector<common::symbol::object_file_t> const &object_files);

    private:
        void first_pass(std::unordered_map<size_t, size_t> &translations,
                        common::symbol::object_file_t const &object_file);

        void second_pass(std::unordered_map<size_t, size_t> &translations,
                         common::symbol::object_file_t const &object_file);

        std::size_t translate_symbol_section(
            const common::symbol::object_file_t &object_file,
            common::symbol::symbol_t const &symbol);

    private:
        common::symbol::symtab_t symtab;
        std::unordered_map<std::string, common::symbol::section_t> sections;
    };
}


#endif //LINKER_H
