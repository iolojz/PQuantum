//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_CONCEPTS_MONOID_HPP
#define CXXMATH_CONCEPTS_MONOID_HPP

#include "monoid_fwd.hpp"
#include "../models/function_object.hpp"

namespace cxxmath
{
namespace concepts
{
template<class Compose, class NeutralElement, class IsAbelian, bool Assignable>
struct monoid
{
	static constexpr bool models_assignable = false;
	
	static constexpr auto compose = function_object_v<Compose>;
	static constexpr auto neutral_element = function_object_v<NeutralElement>;
	static constexpr auto is_abelian_monoid = function_object_v<IsAbelian>;
};

template<class ComposeAssign, class NeutralElement, class IsAbelian>
struct monoid<ComposeAssign, NeutralElement, IsAbelian, true>
{
	static constexpr bool models_assignable = true;
	
	static constexpr auto compose_assign = function_object_v<ComposeAssign>;
	static constexpr auto compose = binary_operator_v<ComposeAssign, IsAbelian>;
	static constexpr auto neutral_element = function_object_v<NeutralElement>;
	static constexpr auto is_abelian_monoid = function_object_v<IsAbelian>;
};
}

template<class DispatchTag, class Compose, class NeutralElement, class IsAbelian>
struct models_concept<DispatchTag, concepts::monoid<Compose, NeutralElement, IsAbelian, false>>
{
	using boolean_lattice = concepts::monoid<Compose, NeutralElement, IsAbelian, false>;
	static constexpr bool value = ( boolean_lattice::compose.template supports_tag<DispatchTag>() &&
									boolean_lattice::neutral_element.template supports_tag<DispatchTag>() &&
									boolean_lattice::is_abelian_monoid.template supports_tag<DispatchTag>());
};

template<class DispatchTag, class ComposeAssign, class NeutralElement, class IsAbelian>
struct models_concept<DispatchTag, concepts::monoid<ComposeAssign, NeutralElement, IsAbelian, true>>
{
	using boolean_lattice = concepts::monoid<ComposeAssign, NeutralElement, IsAbelian, true>;
	static constexpr bool value = ( boolean_lattice::compose_assign.template supports_tag<DispatchTag>() &&
									boolean_lattice::neutral_element.template supports_tag<DispatchTag>() &&
									boolean_lattice::is_abelian_monoid.template supports_tag<DispatchTag>());
};

template<class Tag>
struct default_monoid : no_default_concept<Tag>
{
};
template<class Tag> using default_monoid_t = typename default_monoid<Tag>::type;

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( compose, default_monoid_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( compose_assign, default_monoid_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( is_abelian_monoid, default_monoid_t )
}

#endif //CXXMATH_CONCEPTS_MONOID_HPP
