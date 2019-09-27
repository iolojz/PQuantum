//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_MODELS_PRODUCT_MONOID_HPP
#define CXXMATH_MODELS_PRODUCT_MONOID_HPP

#include "../concepts/monoid.hpp"

#include "../models/std_pair.hpp"

namespace cxxmath
{
template<class FirstTag, class SecondTag>
struct product_monoid_tag;

template<class First, class Second>
struct product_monoid
{
	using dispatch_tag = product_monoid_tag<tag_of_t<First>, tag_of_t<Second>>;
	std::pair<First, Second> pair;
};

template<class Product>
static constexpr auto make_product_monoid( Product &&p )
{
	using first_ = std::decay_t<decltype( first( std::forward<Product>( p )))>;
	using second_ = std::decay_t<decltype( second( std::forward<Product>( p )))>;
	using product_monoid_ = product_monoid<first_, second_>;
	
	return product_monoid_{{ first( std::forward<Product>( p )), second( std::forward<Product>( p )), }};
}

namespace impl
{
namespace detail
{
template<class DispatchTag>
struct is_product_monoid_tag : std::false_type
{
};
template<class FirstTag, class SecondTag>
struct is_product_monoid_tag<product_monoid_tag<FirstTag, SecondTag>> : std::true_type
{
};
}

template<class FirstMonoid, class SecondMonoid>
struct compose_assign_product
{
	template<class DispatchTag>
	static constexpr bool supports_tag( void )
	{
		return detail::is_product_monoid_tag<DispatchTag>::value;
	}
	
	template<class Arg1, class Arg2>
	static constexpr Arg1 &apply( Arg1 &arg1, Arg2 &&arg2 )
	{
		FirstMonoid::compose_assign( arg1.pair.first, std::forward<Arg2>( arg2 ).pair.first );
		SecondMonoid::compose_assign( arg1.pair.second, std::forward<Arg2>( arg2 ).pair.second );
		return arg1;
	}
};

template<class FirstMonoid, class SecondMonoid>
struct compose_product
{
	template<class DispatchTag>
	static constexpr bool supports_tag( void )
	{
		return detail::is_product_monoid_tag<DispatchTag>::value;
	}
	
	template<class Arg1, class Arg2>
	static constexpr auto apply( Arg1 &&arg1, Arg2 &&arg2 )
	{
		auto pair = std::make_pair(
		FirstMonoid::compose( std::forward<Arg1>( arg1 ).pair.first, std::forward<Arg2>( arg2 ).pair.first ),
		SecondMonoid::compose( std::forward<Arg1>( arg1 ).pair.second, std::forward<Arg2>( arg2 ).pair.second ));
		return make_product_monoid( std::move( pair ));
	}
};

template<class FirstMonoid, class SecondMonoid>
struct neutral_element_product
{
	template<class DispatchTag>
	static constexpr bool supports_tag( void )
	{
		return detail::is_product_monoid_tag<DispatchTag>::value;
	}
	
	static constexpr auto apply( void )
	{
		auto pair = std::make_pair( FirstMonoid::neutral_element(), SecondMonoid::neutral_element());
		return make_product_monoid( std::move( pair ));
	}
};

template<class FirstMonoid, class SecondMonoid>
struct is_abelian_monoid_product
{
	template<class DispatchTag>
	static constexpr bool supports_tag( void )
	{
		return detail::is_product_monoid_tag<DispatchTag>::value;
	}
	
	static constexpr auto apply( void )
	{
		return and_( FirstMonoid::is_abelian_monoid(), SecondMonoid::is_abelian_monoid());
	}
};

template<class FirstMonoid, class SecondMonoid>
struct equal_product
{
	template<class DispatchTag>
	static constexpr bool supports_tag( void )
	{
		return detail::is_product_monoid_tag<DispatchTag>::value;
	}
	
	template<class Arg1, class Arg2>
	static constexpr auto apply( Arg1 &&arg1, Arg2 &&arg2 )
	{
		return and_( equal( std::forward<Arg1>( arg1 ).pair.first, std::forward<Arg2>( arg2 ).pair.first ),
					 equal( std::forward<Arg1>( arg1 ).pair.second, std::forward<Arg2>( arg2 ).pair.second ));
	}
};

struct first_product
{
	template<class DispatchTag>
	static constexpr bool supports_tag( void )
	{
		return detail::is_product_monoid_tag<DispatchTag>::value;
	}
	
	template<class Arg>
	static constexpr auto apply( Arg &&arg )
	{
		return first( std::forward<Arg>( arg ).pair );
	}
};

struct second_product
{
	template<class DispatchTag>
	static constexpr bool supports_tag( void )
	{
		return detail::is_product_monoid_tag<DispatchTag>::value;
	}
	
	template<class Arg>
	static constexpr auto apply( Arg &&arg )
	{
		return second( std::forward<Arg>( arg ).pair );
	}
};
}

template<class FirstTag, class SecondTag>
struct default_monoid<product_monoid_tag<FirstTag, SecondTag>>
{
private:
	using default_first_monoid = default_monoid_t<FirstTag>;
	using default_second_monoid = default_monoid_t<SecondTag>;
	
	static constexpr bool models_assignable = ( default_first_monoid::models_assignable &&
												default_second_monoid::models_assignable );
	using composition = std::conditional_t<models_assignable, impl::compose_assign_product<default_first_monoid, default_second_monoid>, impl::compose_product<default_first_monoid, default_second_monoid> >;
	using neutral_element_ = impl::neutral_element_product<default_first_monoid, default_second_monoid>;
	using is_abelian_ = impl::is_abelian_monoid_product<default_first_monoid, default_second_monoid>;
public:
	using type = concepts::monoid<composition, neutral_element_, is_abelian_, models_assignable>;
};

template<class FirstTag, class SecondTag>
struct default_set<product_monoid_tag<FirstTag, SecondTag>>
{
	using type = concepts::set<impl::equal_product<default_monoid_t<FirstTag>, default_monoid_t<SecondTag>>>;
};

template<class FirstTag, class SecondTag>
struct default_product<product_monoid_tag<FirstTag, SecondTag>>
{
	using type = concepts::product<impl::first_product, impl::second_product>;
};
}

#endif //CXXMATH_MODELS_PRODUCT_MONOID_HPP
