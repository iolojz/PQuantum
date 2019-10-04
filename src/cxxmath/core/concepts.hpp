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

template<class DispatchTag>
struct supports_tag_helper
{
	template<class Tag>
	static constexpr bool supports_tag( void )
	{
		return std::is_same_v<DispatchTag, Tag>;
	}
};

template<class ...Implementations>
struct forward_supports_tag
{
	template<class Tag>
	static constexpr bool supports_tag( void )
	{
		return (Implementations::template supports_tag<Tag>() && ... && true);
	}
};

namespace impl
{
struct no_default_concept
{
};
}

#ifdef CXXMATH_DEFINE_CONCEPT
#error "CXXMATH_DEFINE_CONCEPT is already defined"
#endif
#define CXXMATH_DEFINE_CONCEPT( concept ) \
namespace impl { \
template<class Tag, class = void> struct default_ ## concept : no_default_concept {}; \
} \
\
template<class Tag> static constexpr bool has_default_ ## concept = \
    !std::is_base_of_v<impl::no_default_concept, impl::default_ ## concept<Tag>>; \
\
template<class Tag> \
struct default_ ## concept \
{ \
    static_assert( has_default_ ## concept<Tag>, "No default " #concept " for 'Tag' implemented." ); \
    using type = typename impl::default_ ## concept<Tag>::type; \
}; \
template<class Tag> using default_ ## concept ## _t = typename default_ ## concept<Tag>::type;
}

#endif //CXXMATH_CORE_MODELS_CONCEPT_HPP
