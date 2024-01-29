//
// Created by djumi on 1/29/2024.
//

#include "parser_objects.h"

namespace m_asm {
    std::ostream &operator<<(std::ostream &os, const parse_error_t &err) {
        return os << err.file << ":" << err.line << ':' << err.column << ": " << err.message;
    }
}