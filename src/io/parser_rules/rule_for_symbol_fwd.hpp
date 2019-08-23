//
// Created by jayz on 23.08.19.
//

#ifndef PQUANTUM_RULE_FOR_SYMBOL_FWD_HPP
#define PQUANTUM_RULE_FOR_SYMBOL_FWD_HPP

#include <variant>

namespace PQuantum
{
namespace io
{
namespace parser_rules
{
template<class Symbol>
struct rule_for_symbol_impl
{
};
template<class Symbol> constexpr auto rule_for_symbol = rule_for_symbol_impl<Symbol>{};

template<class ...Alternatives>
struct rule_for_symbol_impl<std::variant<Alternatives...>>
{
	template<class Context>
	constexpr auto operator()( Context &&c )
	{
		return (rule_for_symbol<Alternatives>( c ) | ...);
	}
};
}
}
}

#endif //PQUANTUM_RULE_FOR_SYMBOL_FWD_HPP
