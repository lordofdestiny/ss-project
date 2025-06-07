//
// Created by djumi on 11.07.2023..
//

#include <filesystem>
#include "parser_driver.h"

namespace fs = std::filesystem;

namespace m_asm {
    auto parser_driver::parse(fs::path const &f) -> parse_result_t & {
        std::unique_ptr<FILE, std::decay_t<decltype(fclose)> > file(
            std::fopen(absolute(f).c_str(), "r"),
            std::fclose);

        if (!file) {
            throw fs::filesystem_error(
                "cannot open file", f,
                std::make_error_code(std::errc::io_error));
        }

        struct set_yyin {
            explicit set_yyin(const decltype(file) &file) {
                yyin = file.get();
            }

            ~set_yyin() {
                yyin = nullptr;
            }
        } guard(file);
        std::string myf = f;
        loc.initialize(&myf);
        parser.set_debug_level(m_trace_parsing);
        parser.parse();

        if (std::holds_alternative<parse_error_t>(parsed_file)) {
            std::get<parse_error_t>(parsed_file).file = std::move(myf);
        }
        loc.initialize(nullptr);

        return parsed_file;
    }

    void parser_driver::trace_scanning(bool b) {
        m_trace_scanning = b;
    }

    void parser_driver::trace_parsing(bool b) {
        m_trace_parsing = b;
    }
}
