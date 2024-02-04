#include <iostream>
#include <variant>
#include "args.h"

int main(int argc, char **argv) {
    auto const &parsed_args = m_lnk::args::parse_args(argc, argv);
    if (const auto args_ptr = std::get_if<m_lnk::args>(&parsed_args)) {
        std::cout << *args_ptr;
    }
}
