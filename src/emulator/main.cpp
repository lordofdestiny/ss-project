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
    if (argc < 2) {
        std::cerr << "Hex file argument missing.\n";
        return MISSING_ARGS;
    }

    try {
        common::symbol::exec_file_t exec_file;
        std::ifstream ifs(argv[1], std::ios::binary);
        common::util::serde::deserialize(ifs, exec_file);
        ifs.close();

#if DEBUG_PRINT
        std::cout << exec_file << '\n';
#endif

        m_emu::context_t context(exec_file.data);
        emulate(context);

        std::cout << "\nEmulated processor executed halt instruction";
        std::cout << "\nEmulated processor state:\n";
        std::cout << context << std::endl;
    } catch (std::exception &err) {
        std::cerr << err.what() << '\n';
        return RUNTIME_ERROR;
    }
}
