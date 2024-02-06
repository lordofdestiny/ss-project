//
// Created by djumi on 2/5/2024.
//

#include <cstring>
#include "../../include/emulator/context_t.h"

#include <iomanip>
#include <iostream>

namespace m_emu {
    uint32_t context_t::read_word(const uint32_t address) const {
        uint32_t word = 0;

        for (int i = 0; i < 4; i++) {
            word <<= 8;
            if (memory.find(address + i) != memory.end()) {
                word |= memory.at(address + i) & 0xFF;
            }
        }

        std::cout << std::hex << std::setfill('0');
        std::cout << "READ FROM= 0x" << std::setw(8) << address << "; VALUE=0x" << std::setw(8) << word << '\n';
        std::cout << std::dec << std::setfill(' ');

        return word;
    }

    common::instruction_t context_t::read_instr() const {
        const auto raw_bytes = read_word(pc);
        std::cout << std::hex << "PC: " << pc << "; Instr: " << raw_bytes << std::dec << std::endl;
        pc += 4;
        return common::instruction_t::from_word(raw_bytes);
    }

    void context_t::write_to(const uint32_t address, const uint32_t value) {
        if (address == 0xFF'FF'FF'00) {
            std::cout << "OUTPUT: " << static_cast<char>(value & 0xFF) << '\n' << std::flush;
        } else {
            std::cout << std::hex << std::setfill('0');
            std::cout << "WRITE TO= 0x" << std::setw(8) << address << "; VALUE=0x" << std::setw(8) << value << '\n';
            std::cout << std::dec << std::setfill(' ');
        }

        for (uint32_t i = 0; i < 4; i++) {
            memory[address + i] = (value >> (8 * (3 - i))) & 0xFF;
        }
    }

    std::ostream &operator<<(std::ostream &os, context_t const &context) {
        os << std::hex << std::setfill('0');
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                const auto reg = i * 4 + j;

                os << 'r' << reg << "=0x" << std::setw(8) << context.gpr[reg] << '\t';
            }
            os << "\n";
        }

        return os << std::dec << std::setfill(' ');
    }
}
