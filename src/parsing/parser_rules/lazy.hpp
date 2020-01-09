//
// Created by jayz on 09.01.20.
//

#ifndef PQUANTUM_PARSING_PARSER_RULES_RECURSIVE_HPP
#define PQUANTUM_PARSING_PARSER_RULES_RECURSIVE_HPP

#include "../parse.hpp"

namespace PQuantum::parsing::parser_rules {
namespace detail {
template<class ID, class Attr> struct lazy {
	template<class Context>
	constexpr auto operator()( Context context ) const {
		return (boost::spirit::x3::rule<ID, Attr>{ "lazy" } = rule_for<ID>( context ));
	}
};
}

template<class ID, class Attribute> static inline auto make_lazy( void ) {
	return boost::spirit::x3::rule<
		detail::lazy<ID, Attribute>,
		Attribute
	>{};
}

template<class Iterator, class Context, class Attribute, class LazyID, class LazyAttribute>
inline bool parse_rule(
	boost::spirit::x3::rule<detail::lazy<LazyID, LazyAttribute>, LazyAttribute>,
    Iterator &first, const Iterator &last,
    const Context &context, Attribute &attr
) {
	using boost::spirit::x3::get;
	using boost::spirit::x3::unused;
	auto def = rule_for<LazyID>( get<pquantum_context>( context ) );
	return def.parse( first, last, context, unused, attr );
}
}

#endif //PQUANTUM_PARSING_PARSER_RULES_RECURSIVE_HPP
