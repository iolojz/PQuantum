//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_CORE_MODELS_CONCEPT_HPP
#define CXXMATH_CORE_MODELS_CONCEPT_HPP

#include "function_dispatch.hpp"

namespace cxxmath
{
template<class DispatchTag, class Concept>
struct models_concept : std::false_type
{
};
template<class DispatchTag, class Concept> static constexpr bool models_concept_v = models_concept<DispatchTag, Concept>::value;

namespace detail
{
template<class Tag>
struct unsupported_tag
{
	static_assert( always_false_v < Tag > , "The requested function does not support the given tag." );
};
}

template<class DispatchTag>
struct supports_tag_helper
{
	template<class Tag>
	static constexpr bool supports_tag( void )
	{
		return std::is_same_v<DispatchTag, Tag>;
	}
};
}

#endif //CXXMATH_CORE_MODELS_CONCEPT_HPP
