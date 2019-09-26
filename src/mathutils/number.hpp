//
// Created by jayz on 15.08.19.
//

#ifndef PQUANTUM_NUMBER_HPP
#define PQUANTUM_NUMBER_HPP

#include <utility>

#include "abstract_algebra/ring.hpp"
#include "tag_dispatch/core/operator_helpers.hpp"

namespace PQuantum
{
namespace mathutils
{
// TODO: implement numbers...
class number
{
public:
	static number one(void) noexcept { return {}; }
	
	number &operator+=( const number & )
	{ return *this; };
	
	number &operator-=( const number & )
	{ return *this; };
	
	number &operator*=( const number & )
	{ return *this; };
	
	number operator-( void ) const
	{ return *this; };
	
	PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(number, +)
	
	PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(number, *)
	
	PQUANTUM_BINARY_OPERATOR_OVERLOAD(number, -, -)
	
	bool operator==( const number & ) const
	{ return true; }
};

namespace abstract_algebra {
// set of numbers
template<>
struct set_impl<number>
{
	static bool equal( const number &a, const number &b )
	{ return ( a == b ); }
};

// group of numbers under addition (no overflow detection)
template<>
struct group_impl<number>
{
	static constexpr bool is_abelian = true;
	
	template<class Number1, class Number2>
	static decltype( auto ) compose_assign( Number1 &&a, Number2 &&b ) noexcept
	{ return ( std::forward<Number1>( a ) += std::forward<Number2>( b )); }
	
	template<class Number>
	static number inverse( Number &&a ) noexcept
	{ return -std::forward<Number>( a ); }
	
	static number neutral_element( void ) noexcept
	{ return number{}; }
};

// ring of numbers under addition and multiplication (no overflow detection)
template<>
struct ring_impl<number>
{
	template<class Number1, class Number2>
	static decltype( auto ) multiply_assign( Number1 &&a, Number2 &&b ) noexcept
	{ return ( std::forward<Number1>( a ) *= std::forward<Number2>( b )); }
	
	static number one( void ) noexcept
	{ return group<number>::neutral_element(); }
};
}
}
}

#endif //PQUANTUM_NUMBER_HPP
