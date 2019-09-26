//
// Created by jayz on 10.09.19.
//

#ifndef TAGD_CONCEPTS_MONOID_HPP
#define TAGD_CONCEPTS_MONOID_HPP

#include "tag_dispatch/tag_dispatch.hpp"
#include "set.hpp"
#include "makeable.hpp"
#include "tag_dispatch/core/forwarding.hpp"

namespace tag_dispatch {
namespace concepts {
template<class DispatchTag, class StructureTag>
struct monoid;
}

namespace impl {
template<class DispatchTag, class StructureTag>
struct underlying_set_of_monoid;
template<class DispatchTag, class StructureTag> using underlying_set_of_monoid_t = typename underlying_set_of_monoid<DispatchTag, StructureTag>::type;

template<class DispatchTag, class StructureTag>
struct underlying_set_of_monoid<DispatchTag, concepts::monoid<DispatchTag, StructureTag>>
{
	using type = rebind_concept_t<concepts::set, concepts::monoid<DispatchTag, StructureTag>>;
};

TAGD_DEFINE_CONCEPT_FORWARDING_FUNCTION( equal, monoid, equal, underlying_set_of_monoid_t )

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
			return compose_assign<DispatchTag, StructureTag>::apply( arg1, std::forward<Arg2>( arg2 ));
		else if(is_abelian<DispatchTag, StructureTag>::apply() && std::is_rvalue_reference_v<Arg2>)
			return compose_assign<DispatchTag, StructureTag>::apply( arg2, std::forward<Arg1>( arg1 ));
		else {
			auto copy = make<DispatchTag, rebind_concept_t<concepts::makeable, StructureTag>>::apply(
			std::forward<Arg1>( arg1 ));
			return compose_assign<DispatchTag, StructureTag>::apply( copy, std::forward<Arg2>( arg2 ));
		}
	}
};

template<class DispatchTag, class StructureTag>
struct neutral_element : no_impl {
};
}

namespace concepts {
template<class DispatchTag, class StructureTag = monoid<DispatchTag, void>>
struct monoid {
	static constexpr bool value = makeable<DispatchTag, rebind_concept_t<concepts::makeable, StructureTag>>::value &&
								  // FIXME: This leads to undefined instantiations of underlying_set<>! Use SFINAE to avoid this...
								  set<DispatchTag, impl::underlying_set_of_monoid_t<DispatchTag, StructureTag>>::value &&
								  !(std::is_base_of_v<impl::no_impl, impl::is_abelian<DispatchTag, StructureTag>> ||
									std::is_base_of_v<impl::no_impl, impl::compose_assign<DispatchTag, StructureTag>> ||
									std::is_base_of_v<impl::no_impl, impl::neutral_element<DispatchTag, StructureTag>>);
};
}
}

TAGD_CONCEPT_IMPLEMENTS_FUNCTION( monoid, void, is_abelian )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( monoid, void, compose_assign )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( monoid, void, compose )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( monoid, void, neutral_element )

TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(is_abelian)
TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( compose_assign )
TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( compose )
TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(neutral_element)

#endif //TAGD_CONCEPTS_MONOID_HPP
