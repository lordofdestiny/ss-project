//
// Created by djumi on 1/30/2024.
//

#include <algorithm>
#include "assembler.h"
#include "visitors/first_pass.h"
#include "visitors/second_pass.h"

namespace m_asm {
    void assembler::first_pass() {
        visitor::first_pass fpv(std::ref(*this));
        for (const auto &stmt: statements.get()) {
            stmt->accept(fpv);
        }
#ifdef DEBUG_PRINT
        for (const auto &section: sections) {
            std::cout << section << '\n';
        }
#endif
    }

    void assembler::second_pass() {
        visitor::second_pass spv(std::ref(*this));
        for (const auto &stmt: statements.get()) {
            stmt->accept(spv);
        }
#ifdef DEBUG_PRINT
        for (const auto &section: sections) {
            std::cout << section;
        }
#endif
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
        sections.emplace_back(name);
        symbol_table.emplace(name, curr_section, 0, false, 'l',
                             common::symbol::symbol_t::type_t::SECTION);
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
