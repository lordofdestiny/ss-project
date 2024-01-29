//
// Created by djumi on 13.07.2023..
//

#ifndef SS_PROJECT_UTILITY_H
#define SS_PROJECT_UTILITY_H

#include <unordered_map>
#include <ostream>
#include <array>
#include <string>
#include <optional>
#include <variant>
#include <cstdint>
#include <vector>

namespace utility {
    /*
     * overloaded_t is a utility struct used
     * for creating overloaded handlers for std::visit
     * */
    template<class... Ts>
    struct overloaded_t : Ts ... {
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
            std::is_same_v<T, std::remove_cv_t<std::remove_reference_t<U>>
            >, bool>;


    /*
     * Helper type for wrapping visitors ( Visitor pattern)
     * That enables returning values from the visited instances
     * */
    template<typename TVisitor, typename TVisited, typename TResult>
    class value_getter {
    public:
        static TResult get_value(TVisited &visited) {
            TVisitor vis;
            vis->accept(vis);
            return vis.value;
        }

        static TResult get_value(TVisited const &) {
            TVisitor vis;
            vis->accept(vis);
            return vis.value;
        }

        template<typename T,
                enable_type_t<TResult, T> = true>
        void return_value(T &&value_) {
            value = std::forward<T>(value_);
        }

    private:
        TResult value;
    };

    using str_vec_t = std::vector<std::string>;

    bool is_valid_literal(uint32_t literal);
}


#endif //SS_PROJECT_UTILITY_H
