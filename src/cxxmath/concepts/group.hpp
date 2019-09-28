//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_CONCEPTS_GROUP_HPP
#define CXXMATH_CONCEPTS_GROUP_HPP

#include "../models/function_object.hpp"
#include "../core/operator_helpers.hpp"

namespace cxxmath
{
namespace concepts
{
template<class Monoid, class InvertInPlace, class Inverse>
struct group
{
	using monoid_ = Monoid;
	
	static constexpr auto compose = monoid_::compose;
	static constexpr auto compose_assign = monoid_::compose_assign;
	static constexpr auto is_abelian_group = monoid_::is_abelian_monoid;
	static constexpr auto neutral_element = monoid_::neutral_element;
	static constexpr auto invert_in_place = function_object_v<InvertInPlace>;
	static constexpr auto inverse = function_object_v<Inverse>;
};

template<class Monoid, class InvertInPlace> using assignable_group = group<Monoid, InvertInPlace, impl::unary_operator<InvertInPlace>>;

template<class Monoid, class Inverse> using non_assignable_group = group<Monoid, impl::unsupported_implementation, Inverse>;
}

template<class DispatchTag, class Monoid, class InvertInPlace, class Inverse>
struct models_concept<DispatchTag, concepts::group<Monoid, InvertInPlace, Inverse>>
{
private:
	using group = concepts::group<Monoid, InvertInPlace, Inverse>;
	static constexpr bool invert_in_place_valid = std::is_same_v<typename group::invert_in_place::implementation, impl::unsupported_implementation>
												  ? true : group::invert_in_place.template supports_tag<DispatchTag>;
public:
	static constexpr bool value = ( models_concept_v<DispatchTag, Monoid> && invert_in_place_valid &&
									group::inverse.template supports_tag<DispatchTag>());
};

template<class Tag, class = std::void_t<>>
struct default_group : no_default_concept<Tag>
{
};
template<class Tag> using default_group_t = typename default_group<Tag>::type;

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( inverse, default_group_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( is_abelian_group, default_group_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( invert_in_place, default_group_t )
}

#endif //CXXMATH_CONCEPTS_GROUP_HPP
