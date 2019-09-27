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
template<class First, class Second>
struct product
{
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

template<class Tag>
struct default_product : no_default_concept<Tag>
{
};
template<class Tag> using default_product_t = typename default_product<Tag>::type;

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( first, default_product_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( second, default_product_t )
}

#endif //CXXMATH_CONCEPTS_PRODUCT_HPP
