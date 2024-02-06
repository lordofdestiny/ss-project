//
// Created by djumi on 2/5/2024.
//

#include <iomanip>
#include <utility>

#include "../../include/common/exec_file_t.h"

#include <algorithm>
#include <iostream>

namespace common::symbol {
    exec_file_t::exec_file_t(object_file_t const &object_file, places_t const &places) {
        setup_sections(object_file, places);
        setup_symbols(object_file);

        for (auto const &section: object_file.sections) {
            auto const &[s_begin, s_end] = sections[section.name];

            for (size_t i = 0; i < section.data.size(); i++) {
                data.insert_or_assign(s_begin + i, section.data[i]);
            }

            for (const auto &relocation: section.relocations) {
                const auto &symbol = object_file.symtab.begin()[relocation.symbol];
                const auto symbol_address = symbols[symbol.name] + relocation.addend;
                for (int i = 0; i < 4; i++) {
                    data[s_begin + relocation.offset + i] = (symbol_address >> (8 * (3 - i))) & 0xFF;
                }
            }
        }
    }

    void exec_file_t::setup_sections(object_file_t const &object_file, places_t const &places) {
        // Sections with assigned place
        for (const auto &[section_name, section_address]: places) {
            const auto &section_it = std::find_if(
                object_file.sections.begin(),
                object_file.sections.end(),
                [&](const auto &section) { return section.name == section_name; }
            );
            // Section does not exist
            if (section_it == object_file.sections.end()) continue;

            const auto section_size = static_cast<uint32_t>(section_it->data.size());
            const auto section_end = section_address + section_size;

            if (free_address < section_end) {
                free_address = section_end;
            }

            sections[section_name] = section_range_t{section_address, section_end};
        }

        // Assign other sections
        for (const auto &section: object_file.sections) {
            if (places.find(section.name) != places.end()) {
                continue; // Already processed
            }
            const auto section_address = free_address;
            const auto section_size = static_cast<uint32_t>(section.data.size());
            free_address += section_size;

            sections[section.name] = section_range_t{
                section_address, section_address + section_size
            };
        }
    }

    void exec_file_t::setup_symbols(object_file_t const &object_file) {
        for (auto const &symbol: object_file.symtab) {
            const auto &section_name = object_file.symtab.begin()[symbol.section_index].name;
            symbols[symbol.name] = sections[section_name].begin + symbol.value;
        }
    }

    std::ostream &operator<<(std::ostream &os, exec_file_t const &exec_file) {
        if (!exec_file.sections.empty()) {
            os << "Sections: \n";
            os << std::hex << std::setfill('0');
            for (const auto &[name, range]: exec_file.sections) {
                if (name == "$" || name == "") continue;
                os << name << ": [0x" << std::setw(8) << range.begin;
                os << ", 0x" << std::setw(8) << range.end << "]\n";
            }
            os << '\n';
            os << std::dec << std::setfill(' ');
        }
        if (!exec_file.symbols.empty()) {
            os << "Symbols: \n";
            os << std::hex << std::setfill('0');
            for (const auto &[name, addr]: exec_file.symbols) {
                os << name << ": " << "0x" << std::setw(8) << addr << '\n';
            }
            os << '\n';
            os << std::dec << std::setfill(' ');
        }


        os << "Data: \n";
        // Header
        os << std::string(12, ' ');
        os << std::setfill('0') << std::hex;
        for (int i = 0; i < 16; i++) {
            os << std::setw(2) << i << " ";
        }
        os << '\n';

        size_t last_address_group = 0;
        for (const auto &[addr, value]: exec_file.data) {
            if (addr / 16 == last_address_group) {
                continue;
            }
            last_address_group = addr / 16;

            const auto first_address = addr / 16 * 16;
            os << "0x" << std::setw(8) << first_address << ": ";
            for (size_t i = 0; i < 16; i++) {
                if (exec_file.data.find(first_address + i) == exec_file.data.end()) {
                    os << "00";
                } else {
                    os << std::setw(2) << +exec_file.data.at(first_address + i);
                }
                os << " ";
            }
            os << '\n';
            // os <<
        }

        return os << std::dec << std::setfill(' ');
    }
}
