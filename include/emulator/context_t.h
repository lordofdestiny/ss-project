//
// Created by djumi on 2/5/2024.
//

#ifndef CONTEXT_T_H
#define CONTEXT_T_H
#include <cstdint>
#include <map>

#include "../common/instruction_t.h"
#include "../common/util.h"

namespace m_emu {
    struct context_t {
        uint32_t gpr[16] = {};
        uint32_t &sp = gpr[14];
        uint32_t &pc = gpr[15];

        uint32_t csr[3] = {};
        uint32_t &status = csr[0];
        uint32_t &handler = csr[1];
        uint32_t &cause = csr[2];

        std::map<uint32_t, uint8_t> memory;

        template<typename T,
            common::util::enable_type_t<std::map<uint32_t, uint8_t>, T>  = true>
        explicit context_t(T &&memory)
            : memory(std::forward<T>(memory)) {
            gpr[0] = 0;
            pc = 0x40'00'00'00;
            status = 0;
        }

        uint32_t read_word(uint32_t address) const;

        common::instruction_t read_instr() const;

        void write_to(uint32_t address, uint32_t value);

        friend std::ostream &operator<<(std::ostream &os, context_t const &context);
    };

    std::ostream &operator<<(std::ostream &os, context_t const &context);
}

#endif //CONTEXT_T_H
