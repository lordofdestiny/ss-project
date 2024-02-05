//
// Created by djumi on 2/4/2024.
//
#include "../../include/common/serde.h"

namespace common::util::serde {
    void serialize(std::ofstream &ofs, symbol::object_file_t const &object_file) {
        serialize(ofs, object_file.symtab);
        const auto section_size = object_file.sections.size();
        ofs.write(reinterpret_cast<const char *>(&section_size), sizeof(section_size));
        for (const auto &section: object_file.sections) {
            serialize(ofs, section);
        }
    }

    void deserialize(std::ifstream &ifs, symbol::object_file_t &object_file) {
        deserialize(ifs, object_file.symtab);
        size_t sections_count = 0;
        ifs.read(reinterpret_cast<char *>(&sections_count), sizeof(sections_count));
        object_file.sections.resize(sections_count);
        for (auto &section: object_file.sections) {
            deserialize(ifs, section);
        }
    }

    void serialize(std::ofstream &ofs, symbol::exec_file_t const &exec_file) {
        const auto size = exec_file.data.size();
        ofs.write(reinterpret_cast<const char *>(&size), sizeof(size));
        for (const auto &[address, value]: exec_file.data) {
            ofs.write(reinterpret_cast<const char *>(&address), sizeof(address));
            ofs.write(reinterpret_cast<const char *>(&value), sizeof(value));
        }
    }

    void deserialize(std::ifstream &ifs, symbol::exec_file_t &exec_file) {
        size_t size = 0;
        ifs.read(reinterpret_cast<char *>(&size), sizeof(size));
        for (size_t i = 0; i < size; i++) {
            uint32_t address;
            uint8_t value;
            ifs.read(reinterpret_cast<char *>(&address), sizeof(address));
            ifs.read(reinterpret_cast<char *>(&value), sizeof(value));
            exec_file.data[address] = value;
        }
    }

    void serialize(std::ofstream &ofs, symbol::symbol_t const &symbol) {
        auto const &[index, name, section_index, type, value, local, is_equ, has_value] = symbol;
        ofs.write(reinterpret_cast<const char *>(&index), sizeof(index));
        ofs.write(name.data(), name.size() + 1);
        ofs.write(reinterpret_cast<const char *>(&section_index), sizeof(section_index));
        ofs.write(reinterpret_cast<const char *>(&type), sizeof(type));
        ofs.write(reinterpret_cast<const char *>(&value), sizeof(value));
        ofs.write(&local, sizeof(local));
        ofs.write(reinterpret_cast<const char *>(&is_equ), sizeof(is_equ));
        ofs.write(reinterpret_cast<const char *>(&has_value), sizeof(has_value));
    }

    void deserialize(std::ifstream &ifs, symbol::symbol_t &symbol) {
        ifs.read(reinterpret_cast<char *>(&symbol.index), sizeof(symbol.index));
        std::getline(ifs, symbol.name, '\0');
        ifs.read(reinterpret_cast<char *>(&symbol.section_index), sizeof(symbol.section_index));
        ifs.read(reinterpret_cast<char *>(&symbol.type), sizeof(symbol.type));
        ifs.read(reinterpret_cast<char *>(&symbol.value), sizeof(symbol.value));
        ifs.read(&symbol.local, sizeof(symbol.local));
        ifs.read(reinterpret_cast<char *>(&symbol.is_equ), sizeof(symbol.is_equ));
        ifs.read(reinterpret_cast<char *>(&symbol.has_value), sizeof(symbol.has_value));
    }

    void serialize(std::ofstream &ofs, symbol::symtab_t const &symtab) {
        const auto symbols_count = symtab.m_symbols.size();
        ofs.write(reinterpret_cast<const char *>(&symbols_count), sizeof(symbols_count));
        for (const auto &symbol: symtab.m_symbols) {
            serialize(ofs, symbol);
        }
    }

    void deserialize(std::ifstream &ifs, symbol::symtab_t &symtab) {
        size_t symbol_count = 0;
        ifs.read(reinterpret_cast<char *>(&symbol_count), sizeof(symbol_count));
        symtab.m_symbols.resize(symbol_count);
        for (auto &symbol: symtab.m_symbols) {
            deserialize(ifs, symbol);
        }
    }

    void serialize(std::ofstream &ofs, symbol::section_t const &section) {
        ofs.write(reinterpret_cast<const char *>(&section.index), sizeof(section.index));

        ofs.write(section.name.data(), section.name.size() + 1);

        ofs.write(reinterpret_cast<const char *>(&section.size), sizeof(section.size));

        const auto data_size = section.data.size();
        ofs.write(reinterpret_cast<const char *>(&data_size), sizeof(data_size));
        ofs.write(reinterpret_cast<const char *>(section.data.data()), section.data.size());

        const auto relocation_count = section.relocations.size();
        ofs.write(reinterpret_cast<const char *>(&relocation_count), sizeof(relocation_count));
        for (const auto &relocation: section.relocations) {
            serialize(ofs, relocation);
        }
    }

    void deserialize(std::ifstream &ifs, symbol::section_t &section) {
        ifs.read(reinterpret_cast<char *>(&section.index), sizeof(section.index));

        std::getline(ifs, section.name, '\0');

        ifs.read(reinterpret_cast<char *>(&section.size), sizeof(section.size));

        size_t content_size = 0;
        ifs.read(reinterpret_cast<char *>(&content_size), sizeof(content_size));
        section.data.resize(content_size);
        ifs.read(reinterpret_cast<char *>(section.data.data()), content_size);

        size_t relocation_count = 0;
        ifs.read(reinterpret_cast<char *>(&relocation_count), sizeof(relocation_count));
        section.relocations.resize(relocation_count);
        for (auto &relocation: section.relocations) {
            deserialize(ifs, relocation);
        }
    }

    void serialize(std::ofstream &ofs, symbol::relocation_t const &relocation) {
        ofs.write(reinterpret_cast<const char *>(&relocation.offset), sizeof(relocation.offset));
        ofs.write(reinterpret_cast<const char *>(&relocation.symbol), sizeof(relocation.symbol));
        ofs.write(reinterpret_cast<const char *>(&relocation.addend), sizeof(relocation.addend));
    }

    void deserialize(std::ifstream &ifs, symbol::relocation_t &relocation) {
        ifs.read(reinterpret_cast<char *>(&relocation.offset), sizeof(relocation.offset));
        ifs.read(reinterpret_cast<char *>(&relocation.symbol), sizeof(relocation.symbol));
        ifs.read(reinterpret_cast<char *>(&relocation.addend), sizeof(relocation.addend));
    }
}
