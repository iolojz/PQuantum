//
// Created by jayz on 01.10.19.
//

#ifndef CXXMATH_CONCEPTS_R_ALGEBRA_HPP
#define CXXMATH_CONCEPTS_R_ALGEBRA_HPP

namespace cxxmath
{

#include "r_module.hpp"

namespace cxxmath
{
namespace concepts
{
template<class RModule, class Monoid>
struct r_algebra
{
	using r_module_ = RModule;
	using monoid_ = Monoid;
	
	static constexpr auto zero = r_module_::zero;
	static constexpr auto add = r_module_::add;
	static constexpr auto add_assign = r_module_::add_assign;
	static constexpr auto subtract = r_module_::subtract;
	static constexpr auto subtract_assign = r_module_::subtract_assign;
	static constexpr auto negate = r_module_::negate;
	static constexpr auto negate_in_place = r_module_::negate_in_place;
	static constexpr auto scalar_multiply = r_module_::scalar_multiply;
	static constexpr auto scalar_multiply_assign = r_module_::scalar_multiply_assign;
	static constexpr auto multiply = monoid_::multiply;
	static constexpr auto multiply_assign = monoid_::multiply_assign;
	static constexpr auto one = monoid_::one;
};
}

template<class RModule, class Monoid>
struct models_concept<DispatchTag, concepts::r_algebra<RModule, Monoid>>
{
	using r_module = concepts::r_algebra<RModule, Monoid>;
	static constexpr bool value = ( models_concept<DispatchTag, RModule>::value &&
									models_concept<DispatchTag, Monoid>::value );
};

CXXMATH_DEFINE_CONCEPT( r_algebra )
}

#endif //CXXMATH_CONCEPTS_R_ALGEBRA_HPP
