//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_MODELS_FREE_MONOID_HPP
#define CXXMATH_MODELS_FREE_MONOID_HPP

#include "../concepts/monoid.hpp"

#include <vector>

namespace cxxmath
{
template<class Symbol, class Allocator>
struct std_vector_tag;

namespace impl
{
template<class Symbol, class Allocator>
struct tag_of<std::vector<Symbol, Allocator>>
{
	using type = std_vector_tag<Symbol, Allocator>;
};

template<class Symbol, class Allocator>
struct is_abelian_std_vector_tag : supports_tag_helper<std_vector_tag<Symbol, Allocator>>
{
	static constexpr std::bool_constant<std::is_empty_v<Symbol>> apply( void )
	{
		return {};
	}
};

template<class Symbol, class Allocator>
struct neutral_element_std_vector_tag : supports_tag_helper<std_vector_tag<Symbol, Allocator>>
{
	static std::vector<Symbol, Allocator> apply( void )
	{
		return {};
	}
};

template<class Symbol, class Allocator>
struct compose_assign_std_vector_tag : supports_tag_helper<std_vector_tag<Symbol, Allocator>>
{
	static constexpr std::vector<Symbol, Allocator> &
	apply( std::vector<Symbol, Allocator> &v1, const std::vector<Symbol, Allocator> &v2 )
	{
		v1.insert( v1.end(), v2.begin(), v2.end());
		return v1;
	}
	
	static constexpr std::vector<Symbol, Allocator> &
	apply( std::vector<Symbol, Allocator> &v1, std::vector<Symbol, Allocator> &&v2 )
	{
		v1.insert( v1.end(), std::make_move_iterator( v2.begin()), std::make_move_iterator( v2.end()));
		return v1;
	}
};

template<class Symbol, class Allocator>
struct equal_std_vector_tag : supports_tag_helper<std_vector_tag<Symbol, Allocator>>
{
	static constexpr bool apply( const std::vector<Symbol, Allocator> &v1, const std::vector<Symbol, Allocator> &v2 )
	{
		return v1 == v2;
	}
};
}

template<class Symbol, class Allocator>
struct default_monoid<std_vector_tag<Symbol, Allocator>>
{
private:
	using tag = std_vector_tag<Symbol, Allocator>;
	using compose_assign_impl = impl::compose_assign_std_vector_tag<Symbol, Allocator>;
	using neutral_element_impl = impl::neutral_element_std_vector_tag<Symbol, Allocator>;
	using is_abelian_impl = impl::is_abelian_std_vector_tag<Symbol, Allocator>;
public:
	using type = concepts::assignable_monoid<compose_assign_impl, neutral_element_impl, is_abelian_impl>;
};

template<class Symbol, class Allocator>
struct default_set<std_vector_tag<Symbol, Allocator>>
{
	using type = concepts::set<impl::equal_std_vector_tag<Symbol, Allocator>>;
};
}

#endif //CXXMATH_MODELS_FREE_MONOID_HPP
