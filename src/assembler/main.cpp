#include <iostream>
#include <string>
#include <variant>

#include "parser_driver.h"
#include "visitors/printer.h"


int main(int argc, char **argv) {
    using namespace std::string_literals;

    [[maybe_unused]] const char *ofile = "a.out";
    [[maybe_unused]] const char *ifile = "none";
    if (argc < 2) {
        std::cout << "arguments missing\n";
        return 1;
    }
    if (argc == 2) {
        ifile = argv[1];
    } else if (argv[1] != "-o"s) {
        ofile = argv[2];
        ifile = argv[3];
    }

    using m_asm::parser_driver, m_asm::parse_error_t, m_asm::parsed_file_t;

    parser_driver::trace_scanning(false);
    parser_driver::trace_parsing(false);
    parser_driver driver;

    const auto &result = driver.parse(ifile);

    if (const auto *p_val = std::get_if<parse_error_t>(&result)) {
        std::cout << *p_val << "\n";
        return 1;
    }

    const auto &lines = std::get<parsed_file_t>(result);

    m_asm::ast::visitor::printer printer;
    printer.accept(lines);

    std::cout << printer.to_string();

    return 0;
}
