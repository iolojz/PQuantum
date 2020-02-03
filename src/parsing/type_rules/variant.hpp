//
// Created by jayz on 02.02.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULES_VARIANT_HPP
#define PQUANTUM_PARSING_TYPE_RULES_VARIANT_HPP

#include <boost/hana.hpp>

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "../type_rule.hpp"

namespace PQuantum::parsing {
template<class ...Alternatives>
auto evaluate_type_rule(
	boost::hana::basic_type<boost::variant<Alternatives...>>
) {
	static_assert( sizeof...( Alternatives ) != 0 );
	return (lazy_rule_for<typename boost::unwrap_recursive<Alternatives>::type>() | ...);
}
}

#endif //PQUANTUM_PARSING_TYPE_RULES_VARIANT_HPP
