//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_CONCEPTS_SET_HPP
#define CXXMATH_CONCEPTS_SET_HPP

#include "logical.hpp"
#include "monoid.hpp"

namespace cxxmath
{
namespace concepts
{
template<class Equal>
struct set
{
	static constexpr auto equal = function_object_v<Equal>;
	static constexpr auto not_equal = compose( not_, equal );
};
}

template<class DispatchTag, class Equal>
struct models_concept<DispatchTag, concepts::set<Equal>>
{
	using set = concepts::set<Equal>;
	static constexpr bool value = set::equal.template supports_tag<DispatchTag>();
};

CXXMATH_DEFINE_CONCEPT( set )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( equal, default_set_t )

CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( not_equal, default_set_t )
}

#endif //CXXMATH_CONCEPTS_SET_HPP
