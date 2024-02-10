//
// Created by djumi on 2/5/2024.
//

#include "emulator/context_t.h"

#include <iomanip>
#include <iostream>

namespace m_emu {
    uint32_t context_t::read_word(const uint32_t address) const {
        uint32_t word = 0;

        for (int i = 0; i < 4; i++) {
            if (memory.find(address + i) != memory.end()) {
                word |= (memory.at(address + i) & 0xFF) << (i * 8);
            }
        }
        return word;
    }

    common::instruction_t context_t::read_instr() const {
        const auto raw_bytes = read_word(pc);
        pc += 4;
        return common::instruction_t::from_word(raw_bytes);
    }

    void context_t::write_to(const uint32_t address, const uint32_t value) {
        if (address == 0xFF'FF'FF'00) {
            std::cout << static_cast<char>(value & 0xFF) << std::flush;
        }

        for (uint32_t i = 0; i < 4; i++) {
            memory[address + i] = (value >> (8 * i)) & 0xFF;
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
