//
// Created by jayz on 26.11.19.
//

#ifndef PQUANTUM_IO_PARSER_RULES_STD_VARIANT_HPP
#define PQUANTUM_IO_PARSER_RULES_STD_VARIANT_HPP

#include "support/tree.hpp"
#include "support/variant.hpp"

#include <boost/spirit/home/x3.hpp>

namespace PQuantum::parsing::parser_rules {
template<class ...Alternatives>
struct node_rule_for_impl<std::variant<Alternatives...>> {
	static constexpr std::string_view name = "variant";
	
	template<class Context>
	constexpr auto operator()( Context &&c ) const {
		return (node_rule_for<Alternatives>( c ) | ...).operator[]( []( auto &&context ) {
			boost::spirit::x3::_val( context ) = support::to_std_variant<Alternatives...>(
				boost::spirit::x3::_attr( context )
			);
		} );
	}
};
}

#endif //PQUANTUM_IO_PARSER_RULES_STD_VARIANT_HPP
