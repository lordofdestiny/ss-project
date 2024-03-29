//
// Created by djumi on 13.07.2023..
//

#ifndef SS_PROJECT_UTILITY_H
#define SS_PROJECT_UTILITY_H

#include <unordered_map>
#include <string>
#include <variant>
#include <cstdint>
#include <vector>

namespace common::util {
    /*
     * overloaded_t is a utility struct used
     * for creating overloaded handlers for std::visit
     * */
    template<class... Ts>
    struct overloaded_t : Ts... {
        using Ts::operator()...;
    };
    template<typename... Ts>
    overloaded_t(Ts...) -> overloaded_t<Ts...>;

    /*
     * Utility for enabling perfect forwarding of function arguments
     * with type checking at compile type
     * */
    template<typename T, typename U>
    using enable_type_t = std::enable_if_t<
        std::is_same_v<T, std::remove_cv_t<std::remove_reference_t<U> >
        >, bool>;

    using str_vec_t = std::vector<std::string>;

    bool is_valid_literal(uint32_t literal);
    std::string hex_to_string(uint32_t hex);
}


#endif //SS_PROJECT_UTILITY_H
