#include <iostream>
#include <string>
#include <variant>

#include "assembler.h"
#include "parser_driver.h"
#include "visitors/source_printer.h"
#include "assembler.h"

enum asm_result_t {
    OK,
    PARSE_ERROR
};

int main(const int argc, char **argv) {
    using namespace std::string_literals;

    [[maybe_unused]] auto ofile = "a.out";
    [[maybe_unused]] auto *ifile = "none";
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

    // parser_driver::trace_scanning(false);
    // parser_driver::trace_parsing(false);
    parser_driver driver;

    auto &result = driver.parse(ifile);

    if (const auto *p_val = std::get_if<parse_error_t>(&result)) {
        std::cout << *p_val << "\n";
        return PARSE_ERROR;
    }

    auto &parsed_src = std::get<parsed_file_t>(result);

    m_asm::visitor::source_printer printer;
    printer.visit(parsed_src);

    std::cout << printer.to_string();
    m_asm::assembler assembler(std::ref(parsed_src));
    assembler.first_pass();
    std::cout << assembler.get_symbol_table();
    assembler.second_pass();

    return OK;
}
