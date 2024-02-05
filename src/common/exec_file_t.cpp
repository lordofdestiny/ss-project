//
// Created by djumi on 2/5/2024.
//

#include <iomanip>
#include <utility>

#include "../../include/common/exec_file_t.h"

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
                    data[s_begin + relocation.offset + i] = (symbol_address >> (8 * i)) & 0xFF;
                }
            }
        }
    }

    void exec_file_t::setup_sections(object_file_t const &object_file, places_t const &places) {
        for (const auto &section: object_file.sections) {
            auto place_it = places.find(section.name);
            uint32_t section_address;
            if (place_it != places.end()) {
                section_address = place_it->second;

                if (section_address + section.data.size() > free_address) {
                    free_address = place_it->second + section.data.size();
                }
            } else {
                section_address = free_address;
                free_address += section.data.size();
            }
            sections[section.name] = section_range_t{
                section_address,
                section_address + static_cast<uint32_t>(section.data.size())
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
        os << "Sections: \n";
        os << std::hex << std::setfill('0');
        for (const auto &[name, range]: exec_file.sections) {
            if (name == "$" || name == "") continue;
            os << name << ": [0x" << std::setw(8) << range.begin;
            os << ", 0x" << std::setw(8) << range.end << "]\n";
        }
        os << "\nSymbols: \n";
        for (const auto &[name, addr]: exec_file.symbols) {
            os << name << ": " << "0x" << std::setw(8) << addr << '\n';
        }
        os << '\n';
        os << std::dec << std::setfill(' ');

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

        return os;
    }
}
