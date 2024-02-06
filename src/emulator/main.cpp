#include <iomanip>
#include <iostream>

#include "context_t.h"
#include "common/exec_file_t.h"

enum emulator_error {
    OK,
    MISSING_ARGS,
    RUNTIME_ERROR
};

namespace m_emu {
    void emulate(context_t &context);
}

int main(const int argc, char **argv) {
    try {
        if (argc < 2) {
            std::cerr << "Hex file argument missing.\n";
            return MISSING_ARGS;
        }

        common::symbol::exec_file_t exec_file;
        std::ifstream ifs(argv[1], std::ios::binary);
        common::util::serde::deserialize(ifs, exec_file);
        ifs.close();

        std::cout << exec_file << '\n';

        m_emu::context_t context(exec_file.data);
        emulate(context);

        std::cout << "Emulated processor executed halt instruction" << std::endl;
        std::cout << "Emulated processor state:" << std::endl;
        std::cout << context << '\n';
    } catch (std::exception &err) {
        std::cerr << err.what() << '\n';

        return RUNTIME_ERROR;
    }
}
