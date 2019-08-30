//
// Created by jayz on 29.08.19.
//

#ifndef PQUANTUM_GROUP_HPP
#define PQUANTUM_GROUP_HPP

#include "set.hpp"

namespace PQuantum {
namespace mathutils {
namespace abstract_algebra {
template<class GroupElementType, class Tag = void>
struct group_impl;

// group of integers under addition (no overflow detection)
template<>
struct group_impl<int>
{
	static constexpr bool is_abelian = true;
	
	static constexpr int &compose_assign( int &a, int b ) noexcept
	{ return ( a += b ); }
	
	static constexpr int inverse(int a) noexcept { return -a; }
	
	static constexpr int neutral_element(void) noexcept { return 0; }
};

template<class GroupElementType, class Tag = void>
class group : protected set<GroupElementType, Tag>
{
	using base = set<GroupElementType, Tag>;
	using impl = group_impl<GroupElementType, Tag>;
public:
	using tag = Tag;
	
	using base::equal;
	static constexpr bool is_abelian = impl::is_abelian;
	
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( compose_assign, impl, compose_assign )
	
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( inverse, impl, inverse )
	
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( neutral_element, impl, neutral_element )
	
	PQUANTUM_DEFINE_COPYMOVE_STATIC_FUNCTION( compose, GroupElementType, group, compose_assign )
};
}
}
}

#endif //PQUANTUM_GROUP_HPP
