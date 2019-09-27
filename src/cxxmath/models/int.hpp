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
struct is_abelian_ring_int : supports_tag_helper<int>
{
	static constexpr std::true_type apply( void )
	{
		return {};
	}
};

struct zero_int : supports_tag_helper<int>
{
	static constexpr int apply( void )
	{
		return 0;
	}
};

struct one_int : supports_tag_helper<int>
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
}

template<>
struct default_monoid<int>
{
	using type = concepts::monoid<impl::multiply_assign_int, impl::one_int, impl::is_abelian_ring_int, true>;
};
template<>
struct default_group<int>
{
	using type = concepts::group<concepts::monoid<impl::add_assign_int, impl::zero_int, impl::is_abelian_ring_int, true>, impl::negate_in_place_int, true>;
};
template<>
struct default_ring<int>
{
	using type = concepts::ring<default_group_t<int>, default_monoid_t<int>, true>;
};
template<>
struct default_set<int>
{
	using type = concepts::set<impl::equal_int>;
};
}

#endif //CXXMATH_MODELS_INT_HPP