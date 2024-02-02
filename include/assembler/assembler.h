//
// Created by djumi on 1/30/2024.
//

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <utility>

#include "common/section_t.h"
#include "common/symtab_t.h"
#include "parser_driver.h"

namespace m_asm {
    struct section_exception final : std::runtime_error {
        explicit section_exception(const char *msg) : std::runtime_error(msg) {
        }
    };


    class assembler {
    public:
        explicit assembler(const std::reference_wrapper<parsed_file_t> parsed_file)
            : statements(parsed_file) {
        }

        assembler(assembler const &) = delete;

        assembler &operator=(assembler const &) = delete;

        [[nodiscard]] auto &get_symbol_table() { return symbol_table; }

        [[nodiscard]] auto &get_sections() const { return sections; }

        template<typename... T>
        void add_symbol(const bool check, T &&... symbol) {
            if (check && current_section == 0) {
                throw section_exception("no section opened");
            }
            symbol_table.emplace(std::forward<T>(symbol)...);
        }

        [[nodiscard]] uint32_t get_current_section() const {
            return current_section;
        }

        void create_section(const std::string &name);

        void set_current_section(const std::string &name);

        [[nodiscard]] uint32_t get_section_position() const {
            return sections[current_section].size;
        }

        void increment_section_size(const uint32_t size) {
            if (current_section == 0) {
                throw section_exception("no section opened");
            }
            sections[current_section].size += size;
        }

        void write_byte(const uint8_t byte) {
            sections[current_section].data.push_back(byte);
        }

        void write_zeros(const uint32_t count) {
            sections[current_section].data.insert(
                sections[current_section].data.end(), count, 0);
        }

        void write_string(std::string const &string) {
            sections[current_section].data.insert(
                sections[current_section].data.end(),
                string.begin(), string.end());
            sections[current_section].data.push_back(0);
        }

        void write_word(const uint32_t word) {
            write_byte(word & 0xFF);
            write_byte((word >> 8) & 0xFF);
            write_byte((word >> 16) & 0xFF);
            write_byte((word >> 24) & 0xFF);
        }

        void add_relocation(uint64_t offset, uint64_t symbol_index, uint64_t addend) {
            sections[current_section].relocations.emplace_back(offset, symbol_index, addend);
        }

        void first_pass();

        void second_pass();

    private:
        std::reference_wrapper<parsed_file_t> statements;
        uint32_t current_section = 0;
        std::vector<symbols::section_t> sections{
            symbols::section_t{""}
        };
        symbols::symtab_t symbol_table{
            {"", 0, 0, 'l'}
        };
    };
} // m_asm

#endif //ASSEMBLER_H
