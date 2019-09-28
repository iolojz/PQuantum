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
template<class ComposeAssign, class Compose, class NeutralElement, class IsAbelian>
struct monoid
{
	static constexpr auto compose_assign = function_object_v<ComposeAssign>;
	static constexpr auto compose = function_object_v<Compose>;
	static constexpr auto neutral_element = function_object_v<NeutralElement>;
	static constexpr auto is_abelian_monoid = function_object_v<IsAbelian>;
};
}

template<class DispatchTag, class ComposeAssign, class Compose, class NeutralElement, class IsAbelian>
struct models_concept<DispatchTag, concepts::monoid<ComposeAssign, Compose, NeutralElement, IsAbelian>>
{
private:
	using monoid = concepts::monoid<ComposeAssign, Compose, NeutralElement, IsAbelian>;
	static constexpr bool compose_assign_valid = std::is_same_v<typename monoid::compose_assign::implementation, impl::unsupported_implementation>
												 ? true : monoid::compose_assign.template supports_tag<DispatchTag>;
public:
	static constexpr bool value = ( compose_assign_valid && monoid::compose.template supports_tag<DispatchTag>() &&
									monoid::neutral_element.template supports_tag<DispatchTag>() &&
									monoid::is_abelian_monoid.template supports_tag<DispatchTag>());
};

template<class Tag, class>
struct default_monoid : no_default_concept<Tag>
{
};
template<class Tag> using default_monoid_t = typename default_monoid<Tag>::type;

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( compose, default_monoid_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( compose_assign, default_monoid_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( is_abelian_monoid, default_monoid_t )
}

#endif //CXXMATH_CONCEPTS_MONOID_HPP
