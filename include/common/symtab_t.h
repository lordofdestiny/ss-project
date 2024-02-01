//
// Created by djumi on 1/30/2024.
//

#ifndef SS_ALE_SYMTAB_T_H
#define SS_ALE_SYMTAB_T_H

#include <vector>

#include "symbol_t.h"

namespace m_asm::symbols {
    class symtab_t {
    public:
        symtab_t() = default;

        explicit symtab_t(const std::initializer_list<symbol_t> symbols) : m_symbols(symbols) {
        }

        [[nodiscard]] auto begin() { return m_symbols.begin(); }

        [[nodiscard]] auto end() { return m_symbols.end(); }

        [[nodiscard]] auto begin() const { return m_symbols.begin(); }

        [[nodiscard]] auto end() const { return m_symbols.end(); }

        [[nodiscard]] auto cbegin() const { return m_symbols.cbegin(); }

        [[nodiscard]] auto cend() const { return m_symbols.cend(); }

        template<typename... Ts>
        symbol_t &emplace(Ts &&... args) {
            return m_symbols.emplace_back(std::forward<Ts>(args)...);
        }

        [[nodiscard]] auto find(std::string const &symbol_name) -> symbol_t *;

        [[nodiscard]] auto find(std::string const &symbol_name) const -> const symbol_t *;

        friend std::ostream &operator<<(std::ostream &os, symtab_t const &table);

    private:
        std::vector<symbol_t> m_symbols{};
    };

    std::ostream &operator<<(std::ostream &os, symtab_t const &table);
}


#endif //SS_ALE_SYMTAB_T_H
