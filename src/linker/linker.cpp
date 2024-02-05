//
// Created by djumi on 2/5/2024.
//

#include <algorithm>
#include <iterator>
#include <iostream>
#include <unordered_map>
#include "../../include/linker/linker.h"

namespace m_lnk {
    using namespace common::symbol;

    object_file_t linker::link(std::vector<object_file_t> const &object_files) {
        using namespace common::symbol;
        symbol_t::s_index = 0;
        section_t::s_index = 0;

        for (const auto &obj_file: object_files) {
            std::unordered_map<size_t, size_t> translations;
            first_pass(translations, obj_file);
            second_pass(translations, obj_file);
        }

        std::vector<section_t> sections_vector;
        std::transform(sections.begin(), sections.end(),
                       std::back_inserter(sections_vector),
                       [&](const auto &pair) { return pair.second; });

        return {symtab, sections_vector};
    }

    std::size_t linker::translate_symbol_section(const object_file_t &object_file, const symbol_t &symbol) {
        const auto symbol_section = object_file.symtab.begin()[symbol.section_index];
        const auto translated_section = std::find_if(
            symtab.begin(), symtab.end(),
            [&](const auto &s_symbol) {
                return s_symbol.name == symbol_section.name;
            });
        return std::distance(symtab.begin(), translated_section);
    }

    void linker::first_pass(std::unordered_map<size_t, size_t> &translations,
                            object_file_t const &object_file) {
        using namespace common::symbol;
        for (const auto &symbol: object_file.symtab) {
            // For non-exported symbols
            if (symbol.local == 'l' && symbol.type != symbol_t::type_t::SECTION) {
                symbol_t new_symbol{symbol};
                new_symbol.name = "$" + new_symbol.name;

                const auto translated_section_index = translate_symbol_section(object_file, symbol);

                if (const auto translation_it = translations.find(symbol.section_index);
                    translation_it != translations.end()) {
                    new_symbol.value += translation_it->second;
                }
                new_symbol.section_index = translated_section_index;

                symtab.emplace(std::move(new_symbol));
                continue;
            }


            if (const auto duplicate_symbol = symtab.find(symbol.name); duplicate_symbol == nullptr) {
                symbol_t new_symbol{symbol};

                if (symbol.type == symbol_t::type_t::SECTION) {
                    new_symbol.section_index = symtab.size();
                } else if (symbol.section_index != 0) {
                    const auto translated_section_index = translate_symbol_section(object_file, symbol);
                    if (const auto translation_it = translations.find(symbol.section_index);
                        translation_it != translations.end()) {
                        new_symbol.value += translation_it->second;
                    }
                    new_symbol.section_index = translated_section_index;
                }
                symtab.emplace(std::move(new_symbol));
            } else if (duplicate_symbol->type == symbol_t::type_t::SECTION &&
                       symbol.type == symbol_t::type_t::SECTION) {
                const auto &duplicated_section_symbol = sections[duplicate_symbol->name];
                translations[symbol.section_index] = duplicated_section_symbol.data.size();
            } else if (duplicate_symbol->section_index == 0) {
                duplicate_symbol->type = symbol.type;
                duplicate_symbol->value = symbol.value;

                if (symbol.type == symbol_t::type_t::SECTION) {
                    const auto section_it = std::find_if(symtab.begin(), symtab.end(),
                                                         [&](auto const &s_symbol) {
                                                             return s_symbol.name == duplicate_symbol->name;
                                                         });
                    duplicate_symbol->section_index = std::distance(symtab.begin(), section_it);
                } else {
                    const auto translated_section_index = translate_symbol_section(object_file, symbol);
                    duplicate_symbol->section_index = translated_section_index;
                    if (const auto translation_it = translations.find(symbol.section_index);
                        translation_it != translations.end()) {
                        duplicate_symbol->value += translation_it->second;
                    }
                }
            } else if (symbol.section_index != 0) {
                throw std::logic_error("symbol already defined");
            }
        }
    }

    void linker::second_pass(std::unordered_map<size_t, size_t> &translations,
                             object_file_t const &object_file) {
        for (const auto &section: object_file.sections) {
            auto duplicate_section_it = sections.find(section.name);
            auto &new_section = duplicate_section_it == sections.end()
                                    ? (sections[section.name] = section)
                                    : duplicate_section_it->second;
            size_t old_size;
            if (duplicate_section_it == sections.end()) {
                old_size = 0;
                new_section.relocations.clear();
            } else {
                old_size = new_section.data.size();
                new_section.data.insert(
                    new_section.data.end(),
                    section.data.begin(), section.data.end()
                );
            }

            for (const auto &relocation: section.relocations) {
                const auto &original_name = object_file.symtab.begin()[relocation.symbol].name;
                const auto translated_symbol_it = std::find_if(symtab.begin(), symtab.end(),
                                                               [&](auto const &symbol) {
                                                                   return symbol.name == original_name;
                                                               });

                bool und_section = translated_symbol_it == symtab.end();
                const auto translated_symbol_index =
                        und_section ? 0 : std::distance(symtab.begin(), translated_symbol_it);

                relocation_t new_relocation{relocation};
                new_relocation.symbol = translated_symbol_index;
                if (translated_symbol_it->type == symbol_t::type_t::SECTION) {
                    new_relocation.addend += old_size;
                }

                if (const auto translation_it = translations.find(section.index);
                    translation_it != translations.end()) {
                    new_relocation.offset += translation_it->second;
                }

                new_section.relocations.push_back(new_relocation);
            }
        }
    }
}
