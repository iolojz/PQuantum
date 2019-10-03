//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_MODELS_INT_HPP
#define CXXMATH_MODELS_INT_HPP

#include "../concepts/set.hpp"

namespace cxxmath
{
namespace impl
{
struct is_abelian_ring_int
{
	static constexpr std::true_type apply( void )
	{
		return {};
	}
};

struct zero_int
{
	static constexpr int apply( void )
	{
		return 0;
	}
};

struct one_int
{
	static constexpr int apply( void )
	{
		return 1;
	}
};

struct add_assign_int : supports_tag_helper<int>
{
	static constexpr int &apply( int &a, int b )
	{
		return a += b;
	}
};

struct negate_in_place_int : supports_tag_helper<int>
{
	static constexpr int &apply( int &a )
	{
		return a = -a;
	}
};

struct multiply_assign_int : supports_tag_helper<int>
{
	static constexpr int &apply( int &a, int b )
	{
		return a *= b;
	}
};

struct equal_int : supports_tag_helper<int>
{
	static constexpr bool apply( int a, int b )
	{
		return a == b;
	}
};

struct less_int : supports_tag_helper<int>
{
	static constexpr bool apply( int a, int b )
	{
		return a < b;
	}
};

template<>
struct default_monoid<int>
{
	using type = concepts::assignable_monoid<multiply_assign_int, one_int, is_abelian_ring_int>;
};

template<>
struct default_group<int>
{
	using type = concepts::assignable_group <concepts::assignable_monoid<add_assign_int, zero_int, is_abelian_ring_int>, negate_in_place_int>;
};

template<>
struct default_ring<int>
{
	using type = concepts::ring <default_group_t<int>, default_monoid_t<int>>;
};

template<>
struct default_set<int>
{
	using type = concepts::set<equal_int>;
};

template<>
struct default_total_order<int>
{
	using type = concepts::total_order<less_int>;
};
}
}

#endif //CXXMATH_MODELS_INT_HPP
