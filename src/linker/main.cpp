#include <iostream>
#include <variant>
#include <tuple>
#include "args.h"
#include "files.h"

enum lnk_error {
    OK = 0,
    MISSING_FILES = 1,
    BAD_ARGS = 10,
};

int main(int argc, char **argv) {
    auto const &parsed_args = m_lnk::args::parse_args(argc, argv);
    if (const auto args_ptr = std::get_if<m_lnk::args::parse_error>(&parsed_args)) {
        using parse_error = m_lnk::args::parse_error;
        if (*args_ptr == parse_error::MISSING_O_ARG) {
            std::cerr << "Missing file name argument after the '-o' flag.\n";
        } else if (*args_ptr == parse_error::MISSING_PLACE_ARG) {
            std::cerr << "Missing section position argument after the '-place' flag.\n";
        } else if (*args_ptr == parse_error::INVALID_PLACE_FORMAT) {
            std::cerr << "Invalid argument format for '-place' flag.\n";
        } else {
            std::cerr << "Unknow argument error\n";
        }
        return BAD_ARGS + static_cast<int>(*args_ptr);
    }

    const auto &[as_hex, as_reloc, out_file, in_files, places] = std::get<m_lnk::args>(parsed_args);
    const auto files = m_lnk::load_in_files(in_files);
    if (const auto erros_ptr = std::get_if<1>(&files)) {
        for (auto const &error: *erros_ptr) {
            std::cout << error.message;
        }
        return MISSING_FILES;
    }

    const auto &obj_files = std::get<m_lnk::input_files_t>(files);
    for (const auto &obj_file: obj_files) {
        std::cout << obj_file;
    }

    return OK;
}
