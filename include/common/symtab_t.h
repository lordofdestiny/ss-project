//
// Created by djumi on 1/30/2024.
//

#ifndef SS_ALE_SYMTAB_T_H
#define SS_ALE_SYMTAB_T_H

#include <vector>
#include <fstream>
#include <ostream>

#include "symbol_t.h"

namespace common::symbol {
    class symtab_t;
}

namespace common::util::serde {
    void serialize(std::ofstream &ofs, symbol::symtab_t const &symtab);

    void deserialize(std::ifstream &ifs, symbol::symtab_t &symtab);
}

namespace common::symbol {
    class symtab_t {
    public:
        symtab_t() = default;

        [[nodiscard]] auto begin() -> std::vector<symbol_t>::iterator { return m_symbols.begin(); }

        [[nodiscard]] auto end() -> std::vector<symbol_t>::iterator { return m_symbols.end(); }

        [[nodiscard]] auto begin() const -> std::vector<symbol_t>::const_iterator { return m_symbols.begin(); }

        [[nodiscard]] auto end() const -> std::vector<symbol_t>::const_iterator { return m_symbols.end(); }

        [[nodiscard]] auto cbegin() const -> std::vector<symbol_t>::const_iterator { return m_symbols.cbegin(); }

        [[nodiscard]] auto cend() const -> std::vector<symbol_t>::const_iterator { return m_symbols.cend(); }

        [[nodiscard]] auto size() const -> size_t { return m_symbols.size(); }
        [[nodiscard]] auto front() -> symbol_t & { return m_symbols.front(); }
        [[nodiscard]] auto front() const -> symbol_t const & { return m_symbols.front(); }
        [[nodiscard]] auto back() -> symbol_t & { return m_symbols.back(); }
        [[nodiscard]] auto back() const -> symbol_t const & { return m_symbols.back(); }

        template<typename... Ts>
        symbol_t &emplace(Ts &&... args) {
            return m_symbols.emplace_back(std::forward<Ts>(args)...);
        }

        [[nodiscard]] auto find(std::string const &symbol_name) -> symbol_t *;

        [[nodiscard]] auto find(std::string const &symbol_name) const -> const symbol_t *;

        friend void util::serde::serialize(std::ofstream &ofs, symtab_t const &symtab);

        friend void util::serde::deserialize(std::ifstream &ifs, symtab_t &symtab);

        friend std::ostream &operator<<(std::ostream &os, symtab_t const &table);

    private:
        std::vector<symbol_t> m_symbols;
    };

    std::ostream &operator<<(std::ostream &os, symtab_t const &table);
}


#endif //SS_ALE_SYMTAB_T_H
