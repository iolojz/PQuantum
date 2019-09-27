//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_CONCEPTS_RING_HPP
#define CXXMATH_CONCEPTS_RING_HPP

#include "../models/function_object.hpp"

namespace cxxmath
{
namespace concepts
{
template<class AbelianGroup, class Monoid, bool Assignable = false>
struct ring
{
	using abelian_group = AbelianGroup;
	using monoid_ = Monoid;
	
	static constexpr bool models_assignable = false;
	
	static constexpr auto zero = abelian_group::neutral_element;
	static constexpr auto add = abelian_group::compose;
	static constexpr auto negate = abelian_group::inverse;
	
	static constexpr auto is_abelian_ring = monoid_::is_abelian_monoid;
	static constexpr auto one = monoid_::neutral_element;
	static constexpr auto multiply = monoid_::compose;
private:
	using add_impl = typename std::decay_t<decltype( add )>::implementation;
	using negate_impl = typename std::decay_t<decltype( negate )>::implementation;
public:
	static constexpr auto subtract = binary_operator_invert_second_v<add_impl, negate_impl>;
};

template<class AssignableAbelianGroup, class AssignableMonoid>
struct ring<AssignableAbelianGroup, AssignableMonoid, true>
{
	using abelian_group = AssignableAbelianGroup;
	using monoid_ = AssignableMonoid;
	
	static_assert( abelian_group::models_assignable, "The given abelian_group does not model assignable." );
	static_assert( monoid_::models_assignable, "The given monoid does not model assignable." );
	static constexpr bool models_assignable = true;
	
	static constexpr auto zero = abelian_group::neutral_element;
	static constexpr auto add = abelian_group::compose;
	static constexpr auto add_assign = abelian_group::compose_assign;
	static constexpr auto negate = abelian_group::inverse;
	static constexpr auto negate_in_place = abelian_group::invert_in_place;
	static constexpr auto is_abelian_ring = monoid_::is_abelian_monoid;
	static constexpr auto one = monoid_::neutral_element;
	static constexpr auto multiply = monoid_::compose;
	static constexpr auto multiply_assign = monoid_::compose_assign;
private:
	using add_impl = typename std::decay_t<decltype( add )>::implementation;
	using add_assign_impl = typename std::decay_t<decltype( add_assign )>::implementation;
	using negate_impl = typename std::decay_t<decltype( negate )>::implementation;
public:
	static constexpr auto subtract_assign = binary_operator_invert_second_v<add_assign_impl, negate_impl>;
	static constexpr auto subtract = binary_operator_invert_second_v<add_impl, negate_impl>;
};
}

template<class DispatchTag, class AbelianGroup, class Monoid, bool b>
struct models_concept<DispatchTag, concepts::ring<AbelianGroup, Monoid, b>>
{
	using ring = concepts::ring<AbelianGroup, Monoid, b>;
	static constexpr bool value = ( models_concept_v < DispatchTag, AbelianGroup > && models_concept_v < DispatchTag,
	Monoid > );
};

template<class Tag>
struct default_ring : no_default_concept<Tag>
{
};
template<class Tag> using default_ring_t = typename default_ring<Tag>::type;

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( add, default_ring_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( add_assign, default_ring_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( negate, default_ring_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( negate_in_place, default_ring_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( subtract, default_ring_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( subtract_assign, default_ring_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( multiply, default_ring_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( multiply_assign, default_ring_t )
}

#endif //CXXMATH_CONCEPTS_RING_HPP