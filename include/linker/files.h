//
// Created by djumi on 2/5/2024.
//

#ifndef FILES_H
#define FILES_H

#include <string>
#include <vector>
#include <filesystem>

#include "../common/object_file_t.h"

namespace m_lnk {
    using input_files_t = std::vector<common::symbol::object_file_t>;

    struct file_error_t {
        enum class type_t {
            MISSING_FILE
        } type;

        std::filesystem::path file;
        std::string message;

        explicit file_error_t(std::string message, std::filesystem::path file, const type_t type = type_t::MISSING_FILE)
            : type(type), file(std::move(file)), message(std::move(message)) {
        }
    };

    std::variant<input_files_t, std::vector<file_error_t> > load_in_files(std::vector<std::string> const &filenames);
}
#endif //FILES_H
