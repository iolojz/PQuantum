//
// Created by jayz on 29.08.19.
//

#ifndef PQUANTUM_SET_HPP
#define PQUANTUM_SET_HPP

#include "template_helpers/forwarding.hpp"

namespace PQuantum
{
namespace mathutils
{
namespace abstract_algebra
{
template<class SetElementType, class Tag = void>
struct set_impl;

// set of integers
template<>
struct set_impl<int, void>
{
	static constexpr bool equal( int a, int b ) noexcept
	{ return a == b; }
};

template<class SetElementType, class Tag = void>
class set
{
	using impl = set_impl<SetElementType, Tag>;
public:
	using tag = Tag;
	
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( equal, impl, equal )
};
}
}
}

#endif //PQUANTUM_SET_HPP
