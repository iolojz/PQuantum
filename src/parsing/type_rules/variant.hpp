//
// Created by jayz on 02.02.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULES_VARIANT_HPP
#define PQUANTUM_PARSING_TYPE_RULES_VARIANT_HPP

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "../x3_helpers.hpp"

#include <boost/hana/ext/boost/mpl.hpp>

namespace PQuantum::parsing {
template<class ...Alternatives>
struct type_rule_impl<boost::variant<Alternatives...>> {
	static auto apply(void) {
		constexpr auto types = boost::hana::to_tuple( typename boost::variant<Alternatives...>::types{} );
		constexpr auto valid_types = boost::hana::remove( types, boost::hana::type_c<boost::blank> );
		
		static_assert(boost::hana::value( boost::hana::length(valid_types) ) != 0);
		auto rules = boost::hana::transform( valid_types, [] ( auto t ) {
			return make_type_rule<typename boost::unwrap_recursive<typename decltype(+t)::type>::type>();
		} );
		return boost::hana::fold( rules, std::bit_or{} );
	}
};
}

#endif //PQUANTUM_PARSING_TYPE_RULES_VARIANT_HPP
