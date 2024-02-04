//
// Created by djumi on 2/4/2024.
//

#ifndef ARGS_H
#define ARGS_H

#include <string>
#include <variant>
#include <unordered_map>
#include <vector>

namespace m_lnk {
    struct args {
        enum class parse_error {
            MISSING_O_ARG,
            MISSING_PLACE_ARG,
            INVALID_PLACE_FORMAT
        };

        bool hex = false;
        bool reloc = false;
        std::string out_file = "a.out";
        std::vector<std::string> in_files;
        std::unordered_map<std::string, uint32_t> places{};

        friend std::ostream &operator<<(std::ostream &os, const args &args_obj);

        [[maybe_unused]] static std::variant<args, parse_error> parse_args(int argc, char **argv);
    };

    std::ostream &operator<<(std::ostream &os, const args &args_obj);
}

#endif //ARGS_H
