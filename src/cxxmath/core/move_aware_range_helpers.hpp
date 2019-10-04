//
// Created by jayz on 04.10.19.
//

#ifndef CXXMATH_CORE_MOVE_AWARE_RANGE_HELPERS_HPP
#define CXXMATH_CORE_MOVE_AWARE_RANGE_HELPERS_HPP

#include <iterator>
#include <type_traits>

namespace cxxmath
{
template<class Range>
decltype( auto ) move_aware_begin( Range &&r )
{
	if constexpr( std::is_rvalue_reference_v<Range &&> ) {
		return std::make_move_iterator( std::begin( r ));
	} else
		return std::begin( r );
}

template<class Range>
decltype( auto ) move_aware_end( Range &&r )
{
	if constexpr( std::is_rvalue_reference_v<Range &&> ) {
		return std::make_move_iterator( std::end( r ));
	} else
		return std::end( r );
}
}

#endif //CXXMATH_CORE_MOVE_AWARE_RANGE_HELPERS_HPP
