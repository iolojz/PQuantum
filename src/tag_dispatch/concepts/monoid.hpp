//
// Created by jayz on 10.09.19.
//

#ifndef TAGD_CONCEPTS_MONOID_HPP
#define TAGD_CONCEPTS_MONOID_HPP

#include "tag_dispatch/tag_dispatch.hpp"
#include "tag_dispatch/concepts/makeable.hpp"

namespace tag_dispatch {
namespace impl {
template<class DispatchTag, class StructureTag>
struct is_abelian : no_impl {
};

template<class DispatchTag, class StructureTag>
struct compose_assign : no_impl {
};

template<class DispatchTag, class StructureTag>
struct compose {
	template<class Arg1, class Arg2>
	static auto apply(Arg1 &&arg1, Arg2 &&arg2) {
		if constexpr(std::is_rvalue_reference_v<Arg1>)
			return compose_assign<DispatchTag, StructureTag>::apply(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2));
		else if(is_abelian<DispatchTag, StructureTag>::apply() && std::is_rvalue_reference_v<Arg2>)
			return compose_assign<DispatchTag, StructureTag>::apply(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
		else
			return compose_assign<DispatchTag, StructureTag>::apply(
					tag_dispatch::make<DispatchTag>(std::forward<Arg1>(arg1)), std::forward<Arg2>(arg2));
	}
};

template<class DispatchTag, class StructureTag>
struct neutral_element : no_impl {
};
}

namespace concepts {
template<class DispatchTag, class StructureTag>
struct monoid;
template<class DispatchTag, class StructureTag = monoid<DispatchTag, void>>
struct monoid {
	static constexpr bool value = !(std::is_base_of_v<impl::no_impl, impl::is_abelian<DispatchTag, StructureTag>> ||
									std::is_base_of_v<impl::no_impl, impl::compose_assign<DispatchTag, StructureTag>> ||
									std::is_base_of_v<impl::no_impl, impl::neutral_element<DispatchTag, StructureTag>>);
};
}
}

TAGD_CONCEPT_IMPLEMENTS_FUNCTION(monoid, is_abelian)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(monoid, compose_assign)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(monoid, compose)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(monoid, neutral_element)

TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(is_abelian)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(compose_assign)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(compose)
TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(neutral_element)

#endif //TAGD_CONCEPTS_MONOID_HPP
