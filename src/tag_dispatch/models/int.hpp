//
// Created by jayz on 09.09.19.
//

#ifndef TAGD_MODELS_INT_HPP
#define TAGD_MODELS_INT_HPP

#include "tag_dispatch/concepts/ring.hpp"

namespace tag_dispatch::impl
{
template<>
struct make<int, concepts::makeable<int, void>>
{
	static constexpr int apply( int a )
	{ return a; }
};

template<>
struct is_abelian<int, concepts::monoid<int, void>>
{
	static constexpr bool apply( void )
	{ return true; }
};

template<>
struct compose_assign<int, concepts::monoid<int, void>>
{
	static constexpr int &apply( int &a, int b )
	{ return a += b; }
};

template<>
struct neutral_element<int, concepts::monoid<int, void>>
{
	static constexpr int apply( void )
	{ return 0; }
};

template<>
struct inverse_in_place<int, concepts::group<int, void>>
{
	static constexpr int &apply( int &a )
	{ return ( a = -a ); }
};

template<>
struct is_abelian<int, concepts::ring<int, void>>
{
	static constexpr bool apply( void )
	{ return true; }
};

template<>
struct multiply_assign<int, concepts::ring<int, void>>
{
	static constexpr int &apply( int &a, int b )
	{ return a *= b; }
};

template<>
struct one<int, concepts::ring<int, void>>
{
	static constexpr bool apply( void )
	{ return 1; }
};

template<>
struct less<int, concepts::total_order<int, void>>
{
	static constexpr bool apply( int a, int b )
	{ return ( a < b ); }
};

template<>
struct equal<int, concepts::set<int, void>> : equal<int, concepts::total_order<int, void>>
{
};
}

TAGD_MODELLED_CONCEPT_IMPLEMENTS_FUNCTION( int, set, equal )

#endif // TAGD_MODELS_INT_HPP
