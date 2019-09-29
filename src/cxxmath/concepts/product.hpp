//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_CONCEPTS_PRODUCT_HPP
#define CXXMATH_CONCEPTS_PRODUCT_HPP

#include "../models/function_object.hpp"

namespace cxxmath
{
namespace concepts
{
template<class First, class Second, class UniqueFirstTag = void, class UniqueSecondTag = void>
struct product
{
	using unique_first_tag = UniqueFirstTag;
	using unique_second_tag = UniqueSecondTag;
	
	static constexpr bool has_unique_first_tag = !std::is_void_v<unique_first_tag>;
	static constexpr bool has_unique_second_tag = !std::is_void_v<unique_second_tag>;
	
	static constexpr auto first = function_object_v<First>;
	static constexpr auto second = function_object_v<Second>;
};
}

template<class DispatchTag, class First, class Second>
struct models_concept<DispatchTag, concepts::product<First, Second>>
{
	using product = concepts::product<First, Second>;
	static constexpr bool value = ( product::first.template supports_tag<DispatchTag>() &&
									product::second.template supports_tag<DispatchTag>());
};

namespace impl
{
template<class Product>
struct make_product
{
	static_assert( cxxmath::detail::always_false_v<Product>, "make_product not implemented for 'Product'." );
};
}

template<class Product>
struct default_make_product_dispatch
{
	template<class ...Args>
	constexpr decltype( auto ) operator()( Args &&... args ) const
	{
		return impl::make_product<Product>::apply( std::forward<Args>( args )... );
	}
};

template<class Product> static constexpr default_make_product_dispatch<Product> make_product;

template<class Tag>
struct default_product : no_default_concept<Tag>
{
	using type = typename impl::default_product<Tag>::type;
	
};
template<class Tag> using default_product_t = typename default_product<Tag>::type;

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( first, default_product_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( second, default_product_t )
}

#endif //CXXMATH_CONCEPTS_PRODUCT_HPP
