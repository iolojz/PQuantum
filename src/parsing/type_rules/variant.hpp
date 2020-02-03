//
// Created by jayz on 02.02.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULES_VARIANT_HPP
#define PQUANTUM_PARSING_TYPE_RULES_VARIANT_HPP

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

namespace PQuantum::parsing {
template<class ...Alternatives>
struct rule_for_impl<boost::variant<Alternatives...>> {
static constexpr const char *name = "boost_variant";

static constexpr auto apply( void ) {
	static_assert( sizeof...( Alternatives ) != 0 );
	return (rule_for<typename boost::unwrap_recursive<Alternatives>::type>() | ...);
}
};
}

namespace boost::spirit::x3 {
template<class Iterator, class Context, class ...Alternatives>
inline bool parse_rule(
	PQuantum::parsing::type_rule<boost::variant<Alternatives...>>,
	Iterator &first, const Iterator &last,
	const Context &context, boost::variant<Alternatives...> &attr
) {
	static auto const rule_def = PQuantum::parsing::detail::evaluate_type_rule<boost::variant<Alternatives...>>();
	return rule_def.parse( first, last, context, unused, attr );
}
}

#endif //PQUANTUM_PARSING_TYPE_RULES_VARIANT_HPP
