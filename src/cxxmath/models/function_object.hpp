//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_MODELS_FUNCTION_OBJECT_HPP
#define CXXMATH_MODELS_FUNCTION_OBJECT_HPP

#include "../concepts/monoid_fwd.hpp"

namespace cxxmath
{
namespace detail
{
template<class Impl1, class Impl2>
struct compose
{
	template<class Tag>
	static constexpr bool supports_tag( void )
	{
		return Impl2::template supports_tag<Tag>();
	}
	
	template<class ...Args>
	static constexpr decltype( auto ) apply( Args &&...args )
	{
		using intermediate_tag = tag_of_t<decltype( Impl2::apply( std::forward<Args>( args )... ))>;
		static_assert( Impl1::template supports_tag<intermediate_tag>());
		
		return Impl1::apply( Impl2::apply( std::forward<Args>( args )... ));
	}
};

struct identity
{
	template<class>
	static constexpr bool supports_tag( void )
	{ return true; }
	
	template<class Arg>
	static constexpr auto apply( Arg &&arg )
	{
		return arg;
	}
};
}

struct function_object_tag
{
};

template<class Implementation>
struct function_object
{
	using dispatch_tag = function_object_tag;
	using implementation = Implementation;
	
	template<class Tag>
	constexpr bool supports_tag( void ) const
	{
		return Implementation::template supports_tag<Tag>();
	}
	
	template<class ...Args>
	constexpr decltype( auto ) operator()( Args &&...args ) const
	{
		return Implementation::apply( std::forward<Args>( args )... );
	}
};

template<class Implementation> static constexpr auto function_object_v = function_object<Implementation>{};
static constexpr auto identity = function_object_v<detail::identity>;

namespace impl
{
struct compose_function_object
{
	template<class F1, class F2>
	static constexpr auto apply( F1, F2 ) noexcept
	{
		using i1 = typename F1::implementation;
		using i2 = typename F2::implementation;
		
		return function_object_v<detail::compose<i1, i2>>;
	}
};

struct is_abelian_function_object
{
	static constexpr std::false_type apply( void ) noexcept
	{ return {}; }
};

struct neutral_element_function_object
{
	static constexpr auto apply( void ) noexcept
	{ return identity; }
};
}

template<>
struct default_monoid<function_object_tag>
{
	using type = concepts::monoid<impl::compose_function_object, impl::is_abelian_function_object, impl::neutral_element_function_object>;
};
}

#endif //CXXMATH_MODELS_FUNCTION_OBJECT_HPP
