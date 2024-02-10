//
// Created by djumi on 2/4/2024.
//

#include <string>
#include "linker/args.h"

#include <algorithm>
#include <iterator>
#include <sstream>

namespace m_lnk {
    auto args::parse_args(const int argc, char **argv) -> std::variant<args, parse_error> {
        using namespace std::string_literals;

        args args;
        for (int i = 1; i < argc; i++) {
            if (const std::string arg = argv[i]; arg == "-o"s) {
                if (i + 1 >= argc) {
                    return parse_error::MISSING_O_ARG;
                }

                args.out_file = argv[i + 1];
                i++;
            } else if (arg.rfind("-place", 0) == 0) {
                if (arg.length() < 6 || arg[6] != '=' || arg.length() < 7) {
                    return parse_error::INVALID_PLACE_FORMAT;
                }
                const auto place_arg = arg.substr(7);
                const auto delim_index = place_arg.find('@');
                if (delim_index == std::string::npos) {
                    return parse_error::INVALID_PLACE_FORMAT;
                }

                const auto section_name = place_arg.substr(0, delim_index);
                uint32_t address;
                std::stringstream ss;
                ss << std::hex << place_arg.substr(delim_index + 1);
                ss >> address;

                args.places.emplace(section_name, address);
            } else if (arg == "-hex") {
                args.hex = true;
            } else if (arg == "-relocatable") {
                args.reloc = true;
            } else {
                args.in_files.push_back(arg);
            }
        }
        return args;
    }

    std::ostream &operator<<(std::ostream &os, const args &args_obj) {
        os << "In files: ";
        if (!args_obj.in_files.empty()) {
            std::copy(args_obj.in_files.begin(), args_obj.in_files.end() - 1,
                      std::ostream_iterator<std::string>(os, ", "));
            os << args_obj.in_files.back();
        } else {
            os << "none";
        }
        os << std::boolalpha;
        os << "\nOut file: " << args_obj.out_file << '\n';
        os << "Hex: " << args_obj.hex << "\n";
        os << "Relocatable: " << args_obj.reloc << "\n";
        os << "Places: ";
        os << std::hex;
        for (const auto &[section, address]: args_obj.places) {
            os << section << '@' << address << '\n';
        }
        return os << std::dec << std::noboolalpha;
    }
}
