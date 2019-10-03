//
// Created by jayz on 03.10.19.
//

#ifndef CXXMATH_MODELS_STD_GET_HPP
#define CXXMATH_MODELS_STD_GET_HPP

#include "../concepts/product.hpp"

namespace cxxmath
{
namespace impl
{
struct std_get_0
{
	template<class Tag>
	static constexpr bool supports_tag( void )
	{
		return true;
	}
	
	template<class Arg>
	static constexpr decltype( auto ) apply( Arg &&arg )
	{
		return std::get<0>( std::forward<Arg>( arg ));
	}
};

struct std_get_1
{
	template<class Tag>
	static constexpr bool supports_tag( void )
	{
		return true;
	}
	
	template<class Arg>
	static constexpr decltype( auto ) apply( Arg &&arg )
	{
		return std::get<1>( std::forward<Arg>( arg ));
	}
};

template<>
struct make_product<concepts::product < std_get_0, std_get_1>>
{
template<class Tag>
static constexpr bool supports_tag( void )
{
	return true;
}

template<class Arg1, class Arg2>
static constexpr decltype( auto ) apply( Arg1 &&arg1, Arg2 &&arg2 )
{
	return std::make_pair( std::forward<Arg1>( arg1 ), std::forward<Arg2>( arg2 ));
}
};
}

using std_get_product = concepts::product<impl::std_get_0, impl::std_get_1>;
}

#endif //CXXMATH_MODELS_STD_GET_HPP
