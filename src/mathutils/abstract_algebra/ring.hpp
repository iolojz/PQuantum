//
// Created by jayz on 29.08.19.
//

#ifndef PQUANTUM_RING_HPP
#define PQUANTUM_RING_HPP

#include "group.hpp"

namespace PQuantum {
namespace mathutils {
class number;

namespace abstract_algebra {
template<class RingElementType, class Tag = void>
struct ring_impl;

// ring of integers under addition and multiplication (no overflow detection)
template<>
struct ring_impl<int>
{
	static constexpr int &multiply_assign( int &a, int b ) noexcept
	{ return ( a *= b ); }
	
	static constexpr int one(void) noexcept { return 1; }
};

template<class RingElementType, class Tag = void>
class ring : protected group<RingElementType, Tag>
{
	using base = group<RingElementType, Tag>;
	using impl = ring_impl<RingElementType, Tag>;
public:
	static_assert( base::is_abelian, "Underlying group is not abelian." );
	using tag = Tag;
	using base::equal;
	
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( add_assign, base, compose_assign )
	
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( add, base, compose )
	
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( negate, base, inverse )
	
	template<class T1, class T2>
	static decltype( auto ) subtract_assign( T1 &&t1, T2 &&t2 )
	{ return add_assign( std::forward<T1>( t1 ), negate( std::forward<T2>( t2 ))); }
	
	PQUANTUM_DEFINE_COPYMOVE_STATIC_FUNCTION( subtract, RingElementType, ring, subtract_assign )
	
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( multiply_assign, impl, multiply_assign )
	
	PQUANTUM_DEFINE_COPYMOVE_STATIC_FUNCTION( multiply, RingElementType, ring, multiply_assign )
	
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( zero, base, neutral_element )
	
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( one, impl, one )
};
}
}
}

#endif //PQUANTUM_RING_HPP
