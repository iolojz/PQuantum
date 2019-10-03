//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_MODELS_STD_PAIR_HPP
#define CXXMATH_MODELS_STD_PAIR_HPP

#include "../concepts/product.hpp"

#include <utility>

namespace cxxmath
{
struct std_pair_tag;

namespace impl
{
template<class First, class Second>
struct tag_of<std::pair<First, Second>>
{
	using type = std_pair_tag;
};

struct first_std_pair : supports_tag<std_pair_tag>
{
	template<class First, class Second>
	static constexpr auto apply( std::pair<First, Second> &&p )
	{
		return p.first;
	}
	
	template<class First, class Second>
	static constexpr auto &apply( std::pair<First, Second> &p )
	{
		return p.first;
	}
	
	template<class First, class Second>
	static constexpr const auto &apply( const std::pair<First, Second> &p )
	{
		return p.first;
	}
};

struct second_std_pair : supports_tag<std_pair_tag>
{
	template<class First, class Second>
	static constexpr auto apply( std::pair<First, Second> &&p )
	{
		return p.second;
	}
	
	template<class First, class Second>
	static constexpr auto &apply( std::pair<First, Second> &p )
	{
		return p.second;
	}
	
	template<class First, class Second>
	static constexpr const auto &apply( const std::pair<First, Second> &p )
	{
		return p.second;
	}
};

template<>
struct default_product<std_pair_tag>
{
	using type = concepts::product<impl::first_std_pair, impl::second_std_pair>;
};

template<>
struct make_product<default_product_t < std_pair_tag>>
{
template<class Arg1, class Arg2>
static constexpr auto apply( Arg1 &&arg1, Arg2 &&arg2 )
{
	return std::make_pair( std::forward<Arg1>( arg1 ), std::forward<Arg2>( arg2 ));
}
};
}
}

#endif //CXXMATH_MODELS_STD_PAIR_HPP
