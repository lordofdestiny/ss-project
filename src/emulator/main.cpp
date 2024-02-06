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
    // try {
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

    std::cout << std::hex << std::setfill('0');
    std::cout << "value 1: " << std::setw(2) << +context.memory[0] << std::setw(2) << +context.memory[1] << std::setw(2)
            << +context.memory[2] << std::setw(2) << +context.memory[3] << '\n';
    std::cout << "value 2: " << std::setw(2) << +context.memory[4] << std::setw(2) << +context.memory[5] << std::setw(2)
            << +context.memory[6] << std::setw(2) << +context.memory[7] << '\n';
    std::cout << "value 3: " << std::setw(2) << +context.memory[8] << std::setw(2) << +context.memory[9] << std::setw(2)
            << +context.memory[10] << std::setw(2) << +context.memory[11] << '\n';
    std::cout << "value 4: " << std::setw(2) << +context.memory[12] << std::setw(2) << +context.memory[13] <<
            std::setw(2) << +context.memory[14] << std::setw(2) << +context.memory[15] << '\n';
    std::cout << "value 5: " << std::setw(2) << +context.memory[16] << std::setw(2) << +context.memory[17] <<
            std::setw(2) << +context.memory[18] << std::setw(2) << +context.memory[19] << '\n';
    std::cout << "value 6: " << std::setw(2) << +context.memory[20] << std::setw(2) << +context.memory[21] <<
            std::setw(2) << +context.memory[22] << std::setw(2) << +context.memory[23] << '\n';
    std::cout << "value 7: " << std::setw(2) << +context.memory[24] << std::setw(2) << +context.memory[25] <<
            std::setw(2) << +context.memory[26] << std::setw(2) << +context.memory[27] << '\n';
    std::cout << std::dec << std::setfill(' ');

    std::cout << std::hex;
    std::cout << "status: " << context.status << '\n';
    std::cout << "handler: " << context.handler << '\n';
    std::cout << "cause: " << context.cause << '\n';
    std::cout << std::dec;

    std::cout << "Emulated processor executed halt instruction" << std::endl;
    std::cout << "Emulated processor state:" << std::endl;
    std::cout << context << '\n';
    /*} catch (std::exception &err) {
        std::cerr << err.what() << '\n';

        return RUNTIME_ERROR;
    }*/
}
