//
// Created by jayz on 16.01.20.
//

#ifndef PQUANTUM_SUPPORT_STD_UNIQUE_PTR_HPP
#define PQUANTUM_SUPPORT_STD_UNIQUE_PTR_HPP

#include <memory>

#include "parsing_fwd.hpp"

namespace PQuantum::support::parsing {
template<class T>
struct rule_for_impl<std::unique_ptr<T>> {
	static constexpr const char *name = "std_unique_ptr";
	
	static constexpr auto apply( void ) {
		return rule_for<T>().operator[]( []( auto &&x3_context ) {
			boost::spirit::x3::_val( x3_context ) = std::make_unique<T>(
				boost::spirit::x3::_attr( x3_context )
			);
		} );
	}
};
}

#endif //PQUANTUM_SUPPORT_STD_UNIQUE_PTR_HPP
