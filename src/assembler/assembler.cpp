//
// Created by djumi on 1/30/2024.
//

#include <algorithm>
#include "assembler.h"
#include "visitors/first_pass.h"
#include "visitors/second_pass.h"

namespace m_asm {
    common::symbol::object_file_t assembler::assemble() {
        first_pass();
        second_pass();
        return {
            std::move(symbol_table),
            std::move(sections)
        };
    }

    void assembler::first_pass() {
        visitor::first_pass fpv(std::ref(*this));
        for (const auto &stmt: statements.get()) {
            stmt->accept(fpv);
        }
    }

    void assembler::second_pass() {
        visitor::second_pass spv(std::ref(*this));
        for (const auto &stmt: statements.get()) {
            stmt->accept(spv);
        }
    }

    void assembler::create_section(const std::string &name) {
        if (const auto it = std::find_if(
                sections.begin(), sections.end(),
                [&](auto const &section) -> bool {
                    return section.name == name;
                });
            it != sections.end()) {
            throw section_exception(*it == sections.back()
                                        ? "section already opened"
                                        : "cannot reopen closed section"
            );
        }
        curr_section = sections.size();
        symbol_table.emplace(name, symbol_table.size(), 0, false, 'l',
                             common::symbol::symbol_t::type_t::SECTION);
        sections.emplace_back(symbol_table.back().section_index, name);
    }

    void assembler::set_current_section(const std::string &name) {
        const auto it = std::find_if(
            sections.begin(), sections.end(),
            [&](auto const &section) -> bool {
                return section.name == name;
            });
        if (it == sections.end()) {
            throw section_exception("section does not exist");
        }
        curr_section = std::distance(sections.begin(), it);
    }
} // m_asm
