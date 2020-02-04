//
// Created by jayz on 02.02.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULES_VARIANT_HPP
#define PQUANTUM_PARSING_TYPE_RULES_VARIANT_HPP

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "../x3_helpers.hpp"

namespace PQuantum::parsing {
template<class ...Alternatives>
struct type_rule_impl<boost::variant<Alternatives...>> {
	static auto apply(void) {
		static_assert(sizeof...(Alternatives) != 0);
		return (make_type_rule<typename boost::unwrap_recursive<Alternatives>::type>() | ...);
	}
};
}

#endif //PQUANTUM_PARSING_TYPE_RULES_VARIANT_HPP
