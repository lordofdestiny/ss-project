#include <iostream>
#include <string>
#include <variant>
#include <fstream>
#include <filesystem>

#include "assembler.h"
#include "parser_driver.h"
#include "visitors/source_printer.h"
#include "assembler.h"
#include "common/instruction_t.h"
#include "common/util.h"
#include "common/serde.h"

enum asm_result_t {
    OK,
    PARSE_ERROR,
    ARG_ERROR,
};

int main(const int argc, char **argv) {
    using namespace std::string_literals;

    auto ofile = "a.out";
    auto *ifile = "none";
    if (argc < 2) {
        std::cout << "arguments missing\n";
        return ARG_ERROR;
    }
    if (argc == 2) {
        ifile = argv[1];
    } else if (argv[1] == "-o"s && argc == 4) {
        ofile = argv[2];
        ifile = argv[3];
    } else if(argv[2] == "-o"s && argc == 4) {
        ofile = argv[3];
        ifile = argv[1];
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

#if DEBUG_PRINT
    std::cout << printer.to_string();
#endif

    m_asm::assembler assembler(std::ref(parsed_src));
    const auto object_file_data = assembler.assemble();
#if DEBUG_PRINT
    std::cout << object_file_data;
#endif

    std::filesystem::create_directories(std::filesystem::absolute(ofile).parent_path());
    std::ofstream out_stream(ofile, std::ios::binary);
    common::util::serde::serialize(out_stream, object_file_data);
    out_stream.close();

    return OK;
}
