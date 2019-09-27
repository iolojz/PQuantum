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
template<class Monoid, class Inverse, bool Assignable = false>
struct group
{
	using monoid_ = Monoid;
	static constexpr bool models_assignable = false;
	
	static constexpr auto compose = monoid_::compose;
	static constexpr auto is_abelian_group = monoid_::is_abelian_monoid;
	static constexpr auto neutral_element = monoid_::neutral_element;
	
	static constexpr auto inverse = function_object_v<Inverse>;
};

template<class AssignableMonoid, class InvertInPlace>
struct group<AssignableMonoid, InvertInPlace, true>
{
	using monoid_ = AssignableMonoid;
	
	static_assert( monoid_::models_assignable, "The given monoid does not model assignable." );
	static constexpr bool models_assignable = true;
	
	static constexpr auto compose = monoid_::compose;
	static constexpr auto compose_assign = monoid_::compose_assign;
	static constexpr auto is_abelian_group = monoid_::is_abelian_monoid;
	static constexpr auto neutral_element = monoid_::neutral_element;
	
	static constexpr auto invert_in_place = function_object_v<InvertInPlace>;
	static constexpr auto inverse = unary_operator_v<InvertInPlace>;
};
}

template<class DispatchTag, class Monoid, class Inverse>
struct models_concept<DispatchTag, concepts::group<Monoid, Inverse, false>>
{
	using group = concepts::group<Monoid, Inverse, false>;
	static constexpr bool value = ( models_concept_v<DispatchTag, Monoid> &&
									group::inverse.template supports_tag<DispatchTag>());
};

template<class DispatchTag, class Monoid, class Inverse>
struct models_concept<DispatchTag, concepts::group<Monoid, Inverse, true>>
{
	using group = concepts::group<Monoid, Inverse, true>;
	static constexpr bool value = ( models_concept_v<DispatchTag, Monoid> &&
									group::inverse_in_place.template supports_tag<DispatchTag>());
};

template<class Tag>
struct default_group : no_default_concept<Tag>
{
};
template<class Tag> using default_group_t = typename default_group<Tag>::type;

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( inverse, default_group_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( is_abelian_group, default_group_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( invert_in_place, default_group_t )
}

#endif //CXXMATH_CONCEPTS_GROUP_HPP
