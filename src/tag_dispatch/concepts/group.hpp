//
// Created by jayz on 10.09.19.
//

#ifndef TAGD_CONCEPTS_GROUP_HPP
#define TAGD_CONCEPTS_GROUP_HPP

#include "monoid.hpp"
#include "tag_dispatch/forwarding.hpp"

namespace tag_dispatch {
namespace concepts {
template<class DispatchTag, class StructureTag>
struct group;
}

namespace impl {
template<class DispatchTag, class StructureTag>
struct underlying_group;
template<class DispatchTag, class StructureTag> using underlying_group_t = typename underlying_group<DispatchTag, StructureTag>::type;

template<class DispatchTag, class StructureTag>
struct underlying_monoid<DispatchTag, concepts::group<DispatchTag, StructureTag>> {
	using type = rebind_concept_t<concepts::monoid, concepts::group<DispatchTag, StructureTag>>;
};

TAGD_DEFINE_CONCEPT_FORWARDING_FUNCTION(equal, group, equal, underlying_monoid_t)
TAGD_DEFINE_CONCEPT_FORWARDING_FUNCTION(is_abelian, group, is_abelian, underlying_monoid_t)
TAGD_DEFINE_CONCEPT_FORWARDING_FUNCTION(compose_assign, group, compose_assign, underlying_monoid_t)
TAGD_DEFINE_CONCEPT_FORWARDING_FUNCTION(compose, group, compose, underlying_monoid_t)
TAGD_DEFINE_CONCEPT_FORWARDING_FUNCTION(neutral_element, group, neutral_element, underlying_monoid_t)

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
}

namespace concepts {
template<class DispatchTag, class StructureTag = group<DispatchTag, void>>
struct group {
	static constexpr bool value = (monoid<DispatchTag, impl::underlying_monoid_t<DispatchTag, StructureTag>>::value &&
								   !std::is_base_of_v<impl::no_impl, impl::inverse_in_place<DispatchTag, StructureTag>>);
};
}
}

TAGD_CONCEPT_IMPLEMENTS_FUNCTION(group, inverse_in_place)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(group, inverse)

TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(inverse_in_place)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(inverse)

#endif //TAGD_CONCEPTS_GROUP_HPP
