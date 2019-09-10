//
// Created by jayz on 10.09.19.
//

#ifndef TAGD_CONCEPTS_GROUP_HPP
#define TAGD_CONCEPTS_GROUP_HPP

#include "set.hpp"

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
struct inverse_in_place : no_impl {
};

template<class DispatchTag, class StructureTag>
struct inverse {
	template<class Arg>
	static auto apply(Arg &&arg) {
		if constexpr(std::is_rvalue_reference_v<Arg>)
			return inverse_in_place<DispatchTag, StructureTag>::apply(std::forward<Arg>(arg));
		else
			return inverse_in_place<DispatchTag, StructureTag>::apply(
					tag_dispatch::make<DispatchTag>(std::forward<Arg>(arg)));
	}
};

template<class DispatchTag, class StructureTag>
struct neutral_element : no_impl {
};
}

namespace concepts {
template<class DispatchTag, class StructureTag>
struct group;
template<class DispatchTag, class StructureTag = group<DispatchTag, void>>
struct group {
	static constexpr bool value = !(std::is_base_of_v<impl::no_impl, impl::is_abelian<DispatchTag, StructureTag>> ||
									std::is_base_of_v<impl::no_impl, impl::compose_assign<DispatchTag, StructureTag>> ||
									std::is_base_of_v<impl::no_impl, impl::inverse_in_place<DispatchTag, StructureTag>> ||
									std::is_base_of_v<impl::no_impl, impl::neutral_element<DispatchTag, StructureTag>>);
};
}
}

TAGD_CONCEPT_IMPLEMENTS_FUNCTION(group, is_abelian)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(group, compose_assign)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(group, compose)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(group, inverse_in_place)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(group, inverse)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(group, neutral_element)

TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(is_abelian)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(compose_assign)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(compose)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(inverse_in_place)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(inverse)
TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(neutral_element)

#endif //TAGD_CONCEPTS_GROUP_HPP
