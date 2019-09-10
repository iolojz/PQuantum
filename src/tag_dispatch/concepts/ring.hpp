//
// Created by jayz on 10.09.19.
//

#ifndef TAGD_CONCEPTS_RING_HPP
#define TAGD_CONCEPTS_RING_HPP

#include "group.hpp"
#include "tag_dispatch/forwarding.hpp"

namespace tag_dispatch {
namespace concepts {
template<class DispatchTag, class StructureTag>
struct ring;
}

namespace impl {
template<class DispatchTag, class StructureTag>
struct underlying_group;
template<class DispatchTag, class StructureTag>
struct underlying_group<DispatchTag, concepts::ring<DispatchTag, StructureTag>> {
	using type = concepts::group<DispatchTag, StructureTag>;
};
template<class DispatchTag, class StructureTag> using underlying_group_t = typename underlying_group<DispatchTag, StructureTag>::type;

TAGD_DEFINE_FORWARDING_FUNCTION(add_assign, compose_assign, underlying_group_t)

TAGD_DEFINE_FORWARDING_FUNCTION(add, compose, underlying_group_t)

TAGD_DEFINE_FORWARDING_FUNCTION(negate_in_place, inverse_in_place, underlying_group_t)

TAGD_DEFINE_FORWARDING_FUNCTION(negate, inverse, underlying_group_t)

TAGD_DEFINE_FORWARDING_FUNCTION(zero, neutral_element, underlying_group_t)

template<class DispatchTag, class StructureTag>
struct subtract_assign {
	template<class Arg1, class Arg2>
	static auto apply(Arg1 &&arg1, Arg2 &&arg2) {
		if constexpr(std::is_rvalue_reference_v<Arg2>)
			return add_assign<DispatchTag, StructureTag>::apply(std::forward<Arg1>(arg1),
																negate_in_place<DispatchTag, StructureTag>::apply(
																		std::forward<Arg2>(arg2)));
		else if(std::is_rvalue_reference_v<Arg1>)
			return negate_in_place<DispatchTag, StructureTag>::apply(add_assign<DispatchTag, StructureTag>::apply(
					negate_in_place<DispatchTag, StructureTag>::apply(std::forward<Arg1>(arg1)),
					std::forward<Arg2>(arg2)));
		else
			return add_assign<DispatchTag, StructureTag>::apply(std::forward<Arg1>(arg1),
																negate<DispatchTag, StructureTag>::apply(
																		std::forward<Arg2>(arg2)));
	}
};

template<class DispatchTag, class StructureTag>
struct subtract {
	template<class Arg1, class Arg2>
	static auto apply(Arg1 &&arg1, Arg2 &&arg2) {
		if constexpr(std::is_rvalue_reference_v<Arg1>)
			return subtract_assign<DispatchTag, StructureTag>::apply(std::forward<Arg1>(arg1),
																	 std::forward<Arg2>(arg2));
		else if(std::is_rvalue_reference_v<Arg2>)
			return add<DispatchTag, StructureTag>::apply(std::forward<Arg2>(arg1),
														 negate_in_place<DispatchTag, StructureTag>::apply(
																 std::forward<Arg1>(arg2)));
		else
			return subtract_assign<DispatchTag, StructureTag>::apply(
					tag_dispatch::make<DispatchTag>(std::forward<Arg2>(arg1)), std::forward<Arg1>(arg2));
	}
};

template<class DispatchTag, class StructureTag>
struct multiply_assign : no_impl {
};

template<class DispatchTag, class StructureTag>
struct multiply {
	template<class Arg1, class Arg2>
	static auto apply(Arg1 &&arg1, Arg2 &&arg2) {
		if constexpr(std::is_rvalue_reference_v<Arg1>)
			return multiply_assign<DispatchTag, StructureTag>::apply(std::forward<Arg1>(arg1),
																	 std::forward<Arg2>(arg2));
		else if(is_abelian<DispatchTag, StructureTag>::apply() && std::is_rvalue_reference_v<Arg2>)
			return multiply_assign<DispatchTag, StructureTag>::apply(std::forward<Arg2>(arg2),
																	 std::forward<Arg1>(arg1));
		else
			return multiply_assign<DispatchTag, StructureTag>::apply(
					tag_dispatch::make<DispatchTag>(std::forward<Arg1>(arg1)), std::forward<Arg2>(arg2));
	}
};

template<class DispatchTag, class StructureTag>
struct one : no_impl {
};
}

namespace concepts {
template<class DispatchTag, class StructureTag = ring<DispatchTag, void>>
struct ring {
private:
	static constexpr bool is_group_abelian(
			std::true_type) { return is_abelian<DispatchTag, impl::underlying_group_t<DispatchTag, StructureTag>>(); }
	
	static constexpr bool is_group_abelian(std::false_type) { return false; }

public:
	static constexpr bool value = (is_group_abelian(
			std::bool_constant<group<DispatchTag, impl::underlying_group_t<DispatchTag, StructureTag>>::value>{}) &&
								   !(std::is_base_of_v<impl::no_impl, impl::is_abelian<DispatchTag, StructureTag>> ||
									 std::is_base_of_v<impl::no_impl, impl::multiply_assign<DispatchTag, StructureTag>> ||
									 std::is_base_of_v<impl::no_impl, impl::one<DispatchTag, StructureTag>>));
};
}
}

TAGD_CONCEPT_IMPLEMENTS_FUNCTION(ring, is_abelian)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(ring, add_assign)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(ring, add)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(ring, subtract_assign)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(ring, subtract)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(ring, multiply_assign)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(ring, multiply)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(ring, negate_in_place)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(ring, negate)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(ring, zero)
TAGD_CONCEPT_IMPLEMENTS_FUNCTION(ring, one)

TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(add_assign)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(add)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(subtract_assign)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(subtract)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(multiply_assign)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(multiply)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(negate_in_place)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(negate)
TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(zero)
TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(one)

#endif //TAGD_CONCEPTS_RING_HPP
