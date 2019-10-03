//
// Created by jayz on 01.10.19.
//

#ifndef CXXMATH_CONCEPTS_R_MODULE_HPP
#define CXXMATH_CONCEPTS_R_MODULE_HPP

#include "ring.hpp"

namespace cxxmath
{
namespace concepts
{
template<class AbelianGroup, class ScalarMultiplication>
struct r_module
{
	using abelian_group = AbelianGroup;
	using ring_ = Ring;
	using scalar_multiplication = ScalarMultiplication;
	
	static constexpr auto zero = abelian_group::zero;
	static constexpr auto add = abelian_group::compose;
	static constexpr auto add_assign = abelian_group::compose_assign;
	static constexpr auto negate = abelian_group::inverse;
	static constexpr auto negate_in_place = abelian_group::invert_in_place;
	static constexpr auto scalar_multiply = scalar_multiplication::compose;
	static constexpr auto scalar_multiply_assign = scalar_multiplication::compose_assign;
private:
	using add_impl = typename std::decay_t<decltype( add )>::implementation;
	using add_assign_impl = typename std::decay_t<decltype( add_assign )>::implementation;
	using negate_impl = typename std::decay_t<decltype( negate )>::implementation;
public:
	static constexpr auto subtract_assign = function_object_v<std::conditional_t<std::is_same_v<add_assign_impl, impl::unsupported_implementation>, impl::unsupported_implementation, impl::binary_operator_invert_second<add_assign_impl, negate_impl> >>;
	static constexpr auto subtract = binary_operator_invert_second_v<add_impl, negate_impl>;
};
}

template<class DispatchTag, class AbelianGroup, class ScalarMultiplication>
struct models_concept<DispatchTag, concepts::r_module<AbelianGroup, ScalarMultiplication, Ring>>
{
	using r_module = concepts::r_module<AbelianGroup, ScalarMultiplication, Ring>;
	static constexpr bool value = ( models_concept_v<DispatchTag, AbelianGroup> &&
									models_concept_v<DispatchTag, Ring> );
};

CXXMATH_DEFINE_CONCEPT( r_module )

struct default_scalar_multiply_dispatch
{
	template<class Scalar, class Object>
	constexpr decltype( auto ) operator()( Scalar &&scalar, Object &&object ) const
	{
		using dispatch_tag = tag_of_t<Object>;
		return default_r_module<dispatch_tag>::scalar_multiply( std::forward<Scalar>( scalar ),
																std::forward<Object>( object )... );
	}
};

struct default_scalar_multiply_assign_dispatch
{
	template<class Scalar, class Object>
	constexpr decltype( auto ) operator()( Scalar &&scalar, Object &&object ) const
	{
		using dispatch_tag = tag_of_t<Object>;
		return default_r_module<dispatch_tag>::scalar_multiply_assign( std::forward<Scalar>( scalar ),
																	   std::forward<Object>( object )... );
	}
};

static constexpr default_scalar_multiply_dispatch scalar_multiply;
static constexpr default_scalar_multiply_assign_dispatch scalar_multiply_assign;
}

#endif //CXXMATH_CONCEPTS_R_MODULE_HPP
