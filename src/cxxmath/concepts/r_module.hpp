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
template<class AbelianGroup, class ScalarMultiplyAssign, class ScalarMultiply>
struct r_module
{
	using abelian_group = AbelianGroup;
	
	static constexpr auto zero = abelian_group::zero;
	static constexpr auto add = abelian_group::compose;
	static constexpr auto add_assign = abelian_group::compose_assign;
	static constexpr auto negate = abelian_group::inverse;
	static constexpr auto negate_in_place = abelian_group::invert_in_place;
	static constexpr auto scalar_multiply = function_object_v<ScalarMultiply>;
	static constexpr auto scalar_multiply_assign = function_object_v<ScalarMultiplyAssign>;
private:
	using add_impl = typename std::decay_t<decltype( add )>::implementation;
	using add_assign_impl = typename std::decay_t<decltype( add_assign )>::implementation;
	using negate_impl = typename std::decay_t<decltype( negate )>::implementation;
public:
	static constexpr auto subtract_assign = function_object_v<std::conditional_t<std::is_same_v<add_assign_impl, impl::unsupported_implementation>, impl::unsupported_implementation, impl::binary_operator_invert_second<add_assign_impl, negate_impl> >>;
	static constexpr auto subtract = binary_operator_invert_second_v<add_impl, negate_impl>;
};

namespace detail
{
template<class ScalarMultiplyAssign>
struct scalar_multiply : forward_supports_tag<ScalarMultiplyAssign>
{
	template<class Scalar, class Object>
	static constexpr decltype( auto ) apply( Scalar &&s, Object &&o )
	{
		if constexpr( std::is_rvalue_reference_v<Object &&> )
			return ScalarMultiplyAssign::apply( std::forward<Scalar>( s ), std::forward<Object>( o ));
		else {
			auto copy = std::decay_t<Object>{ std::forward<Object>( o ) };
			ScalarMultiplyAssign::apply( std::forward<Scalar>( s ), copy );
			return copy;
		}
	}
};
}

template<class AbelianGroup, class ScalarMultiplyAssign> using assignable_r_module = r_module<AbelianGroup, ScalarMultiplyAssign, detail::scalar_multiply<ScalarMultiplyAssign>>;
template<class AbelianGroup, class ScalarMultiply> using non_assignable_r_module = r_module<AbelianGroup, impl::unsupported_implementation, ScalarMultiply>;
}

template<class DispatchTag, class AbelianGroup, class ScalarMultiplyAssign, class ScalarMultiply>
struct models_concept<DispatchTag, concepts::r_module<AbelianGroup, ScalarMultiplyAssign, ScalarMultiply>>
{
	using r_module = concepts::r_module<AbelianGroup, ScalarMultiplyAssign, ScalarMultiply>;
	static constexpr bool value = models_concept_v<DispatchTag, AbelianGroup>;
};

CXXMATH_DEFINE_CONCEPT( r_module )

struct default_scalar_multiply_dispatch
{
	template<class Scalar, class Object>
	constexpr decltype( auto ) operator()( Scalar &&scalar, Object &&object ) const
	{
		using dispatch_tag = tag_of_t<Object>;
		return default_r_module_t<dispatch_tag>::scalar_multiply( std::forward<Scalar>( scalar ),
																  std::forward<Object>( object ));
	}
};

struct default_scalar_multiply_assign_dispatch
{
	template<class Scalar, class Object>
	constexpr decltype( auto ) operator()( Scalar &&scalar, Object &&object ) const
	{
		using dispatch_tag = tag_of_t<Object>;
		return default_r_module_t<dispatch_tag>::scalar_multiply_assign( std::forward<Scalar>( scalar ),
																		 std::forward<Object>( object ));
	}
};

static constexpr default_scalar_multiply_dispatch scalar_multiply;
static constexpr default_scalar_multiply_assign_dispatch scalar_multiply_assign;
}

#endif //CXXMATH_CONCEPTS_R_MODULE_HPP
