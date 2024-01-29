//
// Created by djumi on 11.07.2023...
//

#ifndef SS_PROJECT_PARSER_DRIVER_H
#define SS_PROJECT_PARSER_DRIVER_H

#include <string>
#include <optional>
#include <filesystem>
#include <variant>
#include "parser_objects.h"
#include "parser.hh"

extern int yy_flex_debug;
extern FILE* yyin;

namespace m_asm {
    class parser_driver;
    class bison_parser;

    bison_parser::symbol_type yylex(parser_driver& driver);

    class parser_driver {
    public:
        friend class m_asm::bison_parser;

        explicit parser_driver() {
            yy_flex_debug = m_trace_scanning;
        };

        parse_result_t& parse(std::filesystem::path const& f);

        static void trace_scanning(bool b);

        static void trace_parsing(bool b);

        m_asm::location location;
    private:
        parse_result_t parsed_file{};
        static inline bool m_trace_parsing = false;
        static inline bool m_trace_scanning = false;
        bison_parser parser{*this};
    };
}


#endif //SS_PROJECT_PARSER_DRIVER_H
