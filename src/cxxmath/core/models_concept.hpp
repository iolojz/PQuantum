//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_CORE_MODELS_CONCEPT_HPP
#define CXXMATH_CORE_MODELS_CONCEPT_HPP

#include "../models/function_object.hpp"

namespace cxxmath
{
template<class DispatchTag, class Concept>
struct models_concept : std::false_type
{
};
template<class DispatchTag, class Concept> static constexpr bool models_concept_v = models_concept<DispatchTag, Concept>::value;

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
