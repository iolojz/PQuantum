//
// Created by jayz on 23.08.19.
//

#ifndef PQUANTUM_RULE_FOR_SYMBOL_FWD_HPP
#define PQUANTUM_RULE_FOR_SYMBOL_FWD_HPP

#include <variant>

namespace PQuantum::io::parser_rules
{
template<class Symbol>
struct rule_for_symbol_impl
{
};
template<class Symbol> constexpr auto rule_for_symbol = rule_for_symbol_impl<Symbol>{};
}

#endif //PQUANTUM_RULE_FOR_SYMBOL_FWD_HPP
