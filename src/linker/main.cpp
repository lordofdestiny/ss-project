#include <iostream>
#include <variant>
#include <tuple>
#include "args.h"
#include "files.h"
#include "linker.h"
#include "common/exec_file_t.h"

enum lnk_error {
    OK = 0,
    MISSING_FILES = 1,
    HEX_AND_RELOC = 2,
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

    const auto &args = std::get<m_lnk::args>(parsed_args);
    const auto &[as_hex, as_reloc, out_file, in_files, places] = args;

    if ((!as_hex && !as_reloc) || (as_hex && as_reloc)) {
        std::cerr << "One and only option between -hex and -relocatable must be chosen.\n";
        return HEX_AND_RELOC;
    }

    const auto files = m_lnk::load_in_files(in_files);
    if (const auto erros_ptr = std::get_if<1>(&files)) {
        for (auto const &error: *erros_ptr) {
            std::cout << error.message;
        }
        return MISSING_FILES;
    }
    try {
        const auto &obj_files = std::get<m_lnk::input_files_t>(files);
#if DEBUG_PRINT
        int obj_file_cnt = 1;
        for (const auto &obj_file: obj_files) {
            std::cout << "File " << obj_file_cnt << ": \n";
            std::cout << obj_file;
            obj_file_cnt++;
        }
#endif

        m_lnk::linker linker;
        const auto obj_file = linker.link(obj_files);
#if DEBUG_PRINT
        std::cout << "Result: \n";
        std::cout << obj_file;
#endif

        if (as_reloc) {
            std::ofstream ofs(out_file, std::ios::binary);
            common::util::serde::serialize(ofs, obj_file);
            ofs.close();
        } else if (as_hex) {
            common::symbol::exec_file_t exec_file(obj_file, places);
            std::cout << exec_file;
            std::ofstream ofs(out_file, std::ios::binary);
            common::util::serde::serialize(ofs, exec_file);
            ofs.close();
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return OK;
}
