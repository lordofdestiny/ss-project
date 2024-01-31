//
// Created by lod on 7/10/23.
//

#ifndef SS_PROJECT_PARSER_OBJECTS_H
#define SS_PROJECT_PARSER_OBJECTS_H

#include <vector>
#include <cstdint>
#include <variant>
#include <optional>
#include <string>
#include <vector>
#include <array>
#include <ostream>
#include <unordered_map>

#include "ast/stmt_t.h"

namespace m_asm {
    using line_content_t = std::unique_ptr<m_asm::ast::stmt_t>;
    using parsed_file_t = std::vector<line_content_t>;

    struct parse_error_t {
        std::string file;
        std::string message;
        int line, column;

        friend std::ostream &operator<<(std::ostream &os, parse_error_t const &err);
    };

    std::ostream &operator<<(std::ostream &os, parse_error_t const &err);

    using parse_result_t = std::variant<std::monostate, parsed_file_t, parse_error_t>;
}


#endif //SS_PROJECT_PARSER_OBJECTS_H
