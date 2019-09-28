//
// Created by jayz on 09.09.19.
//

#ifndef CXXMATH_MODELS_BOOL_HPP
#define CXXMATH_MODELS_BOOL_HPP

#include "../concepts/set.hpp"

namespace cxxmath
{
namespace impl {
struct and_bool : supports_tag_helper<bool>
{
	static constexpr bool apply( bool b1, bool b2 ) noexcept
	{
		return b1 && b2;
	}
};

struct or_bool : supports_tag_helper<bool>
{
	static constexpr bool apply( bool b1, bool b2 ) noexcept
	{
		return b1 || b2;
	}
};

struct not_bool : supports_tag_helper<bool>
{
	static constexpr bool apply( bool b ) noexcept
	{
		return !b;
	}
};

struct if_bool
{
	template<class Then, class Else>
	static constexpr auto apply( bool b, Then &&then, Else &&else_ )
	{
		return b ? then : else_;
	}
};

struct equal_bool : supports_tag_helper<bool>
{
	static constexpr bool apply( bool b1, bool b2 ) noexcept
	{
		return b1 == b2;
	}
};
}

template<>
struct default_boolean_lattice<bool>
{
	using type = concepts::boolean_lattice<impl::and_bool, impl::or_bool, impl::not_bool>;
};

template<>
struct default_logical<bool>
{
	using type = concepts::logical<default_boolean_lattice<bool>, impl::if_bool>;
};

template<>
struct default_set<bool>
{
	using type = concepts::set<impl::equal_bool>;
};

struct std_bool_constant_tag
{
};

namespace impl
{
template<>
struct tag_of<std::true_type>
{
	using type = std_bool_constant_tag;
};
template<>
struct tag_of<std::false_type>
{
	using type = std_bool_constant_tag;
};

struct and_std_bool_constant : supports_tag_helper<std_bool_constant_tag>
{
	template<bool b1, bool b2>
	static constexpr std::bool_constant<b1 && b2> apply( std::bool_constant<b1>, std::bool_constant<b2> ) noexcept
	{ return {}; }
};

struct or_std_bool_constant : supports_tag_helper<std_bool_constant_tag>
{
	template<bool b1, bool b2>
	static constexpr std::bool_constant<b1 || b2> apply( std::bool_constant<b1>, std::bool_constant<b2> ) noexcept
	{ return {}; }
};

struct not_std_bool_constant : supports_tag_helper<std_bool_constant_tag>
{
	template<bool b>
	static constexpr std::bool_constant<!b> apply( std::bool_constant<b> ) noexcept
	{ return {}; }
};

struct if_std_bool_constant
{
	template<class Then, class Else>
	static constexpr auto apply( std::true_type, Then &&then, Else && )
	{
		return then;
	}
	
	template<class Then, class Else>
	static constexpr auto apply( std::false_type, Then &&, Else &&else_ )
	{
		return else_;
	}
};

struct equal_std_bool_constant : supports_tag_helper<std_bool_constant_tag>
{
	template<bool b1, bool b2>
	static constexpr std::bool_constant<b1 == b2> apply( std::bool_constant<b1>, std::bool_constant<b2> ) noexcept
	{ return {}; }
};
}

template<>
struct default_boolean_lattice<std_bool_constant_tag>
{
	using type = concepts::boolean_lattice<impl::and_std_bool_constant, impl::or_std_bool_constant, impl::not_std_bool_constant>;
};

template<>
struct default_logical<std_bool_constant_tag>
{
	using type = concepts::logical <default_boolean_lattice_t<std_bool_constant_tag>, impl::if_std_bool_constant>;
};

template<>
struct default_set<std_bool_constant_tag>
{
	using type = concepts::set<impl::equal_std_bool_constant>;
};
}

#endif //CXXMATH_MODELS_BOOL_HPP
