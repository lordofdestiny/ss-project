//
// Created by djumi on 2/5/2024.
//

#include <algorithm>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "linker/files.h"

namespace m_lnk {
    std::variant<input_files_t, std::vector<file_error_t> > load_in_files(
        std::vector<std::string> const &filenames) {
        namespace fs = std::filesystem;
        using namespace std::string_literals;
        std::vector<fs::path> absolute_paths;
        absolute_paths.reserve(filenames.size());
        std::transform(filenames.begin(), filenames.end(),
                       std::back_inserter(absolute_paths),
                       [](const auto &path) { return fs::absolute(path); });

        std::vector<file_error_t> missing_files;
        for (const auto &path: absolute_paths) {
            if (!fs::exists(path)) {
                std::stringstream ss;
                ss << "File " << path << " does not exist.";
                missing_files.emplace_back(ss.str(), path);
            }
        }

        if (!missing_files.empty()) {
            return missing_files;
        }

        input_files_t files;
        std::transform(filenames.begin(), filenames.end(),
                       std::inserter(files, files.begin()),
                       [&](auto const &filename) {
                           std::ifstream ifs{filename, std::ios::binary};
                           common::symbol::object_file_t obj_file;
                           common::util::serde::deserialize(ifs, obj_file);
                           ifs.close();
                           return obj_file;
                       });

        return files;
    }
}
