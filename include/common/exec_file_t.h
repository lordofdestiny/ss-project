//
// Created by djumi on 2/5/2024.
//

#ifndef EXEC_FILE_T_H
#define EXEC_FILE_T_H
#include <map>
#include <unordered_map>

#include "object_file_t.h"

namespace common::symbol {
    struct exec_file_t;
}

namespace common::util::serde {
    void serialize(std::ofstream &ofs, symbol::exec_file_t const &object_file);

    void deserialize(std::ifstream & ifs, symbol::exec_file_t & object_file);
}

namespace common::symbol {
    struct exec_file_t {
        using places_t = std::unordered_map<std::string, uint32_t>;

        exec_file_t() = default;

        exec_file_t(object_file_t const &object_file, places_t const &places);

        friend void util::serde::serialize(std::ofstream &ofs, exec_file_t const &object_file);

        friend void util::serde::deserialize(std::ifstream &ifs, exec_file_t &object_file);


        friend std::ostream &operator<<(std::ostream &os, exec_file_t const &exec_file);

    private:
        struct section_range_t {
            uint32_t begin, end;
        };

        void setup_sections(object_file_t const &object_file, places_t const &places);

        void setup_symbols(object_file_t const &object_file);

        uint32_t free_address = 0;
        std::map<std::string, section_range_t> sections;
        std::map<std::string, uint32_t> symbols;
        std::map<uint32_t, uint8_t> data;
    };

    std::ostream &operator<<(std::ostream &os, exec_file_t const &exec_file);
}

#endif //EXEC_FILE_T_H
