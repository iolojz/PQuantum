//
// Created by jayz on 10.09.19.
//

#ifndef TAGD_CONCEPTS_RING_HPP
#define TAGD_CONCEPTS_RING_HPP

#include "group.hpp"
#include "tag_dispatch/core/forwarding.hpp"

namespace tag_dispatch {
namespace concepts {
template<class DispatchTag, class StructureTag>
struct ring;
}

namespace impl {
template<class DispatchTag, class StructureTag>
struct underlying_group_of_ring;
template<class DispatchTag, class StructureTag> using underlying_group_of_ring_t = typename underlying_group_of_ring<DispatchTag, StructureTag>::type;

template<class DispatchTag, class StructureTag>
struct underlying_group_of_ring<DispatchTag, concepts::ring<DispatchTag, StructureTag>>
{
	using type = rebind_concept_t<concepts::group, concepts::ring<DispatchTag, StructureTag>>;
};

TAGD_DEFINE_CONCEPT_FORWARDING_FUNCTION( equal, ring, equal, underlying_group_of_ring_t )

TAGD_DEFINE_FORWARDING_FUNCTION( add_assign, compose_assign, underlying_group_of_ring_t )

TAGD_DEFINE_FORWARDING_FUNCTION( add, compose, underlying_group_of_ring_t )

TAGD_DEFINE_FORWARDING_FUNCTION( negate_in_place, inverse_in_place, underlying_group_of_ring_t )

TAGD_DEFINE_FORWARDING_FUNCTION( negate, inverse, underlying_group_of_ring_t )

TAGD_DEFINE_FORWARDING_FUNCTION( zero, neutral_element, underlying_group_of_ring_t )

template<class DispatchTag, class StructureTag>
struct subtract_assign {
	template<class Arg1, class Arg2>
	static decltype( auto ) apply( Arg1 &arg1, Arg2 &&arg2 )
	{
		if constexpr(std::is_rvalue_reference_v<Arg2>)
			return add_assign<DispatchTag, StructureTag>::apply( arg1,
																 negate_in_place<DispatchTag, StructureTag>::apply(
																 arg2 ));
		else
			return add_assign<DispatchTag, StructureTag>::apply( arg1, negate<DispatchTag, StructureTag>::apply(
			std::forward<Arg2>( arg2 )));
	}
};

template<class DispatchTag, class StructureTag>
struct subtract {
	template<class Arg1, class Arg2>
	static auto apply(Arg1 &&arg1, Arg2 &&arg2) {
		if constexpr(std::is_rvalue_reference_v<Arg1>)
			return subtract_assign<DispatchTag, StructureTag>::apply( arg1, std::forward<Arg2>( arg2 ));
		else if(std::is_rvalue_reference_v<Arg2>)
			return add<DispatchTag, StructureTag>::apply( std::forward<Arg1>( arg1 ),
														  negate_in_place<DispatchTag, StructureTag>::apply( arg2 ));
		else {
			auto copy = make<DispatchTag, rebind_concept_t<concepts::makeable, StructureTag>>::apply(
			std::forward<Arg1>( arg1 ));
			return subtract_assign<DispatchTag, StructureTag>::apply( copy, std::forward<Arg2>( arg2 ));
		}
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
			return multiply_assign<DispatchTag, StructureTag>::apply( arg1, std::forward<Arg2>( arg2 ));
		else if(is_abelian<DispatchTag, StructureTag>::apply() && std::is_rvalue_reference_v<Arg2>)
			return multiply_assign<DispatchTag, StructureTag>::apply( arg2, std::forward<Arg1>( arg1 ));
		else {
			auto copy = make<DispatchTag, rebind_concept_t<concepts::makeable, StructureTag>>::apply(
			std::forward<Arg1>( arg1 ));
			return multiply_assign<DispatchTag, StructureTag>::apply( copy, std::forward<Arg2>( arg2 ));
		}
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
	static constexpr bool is_group_abelian( std::true_type )
	{ return is_abelian<DispatchTag, impl::underlying_group_of_ring_t<DispatchTag, StructureTag>>(); }
	
	static constexpr bool is_group_abelian(std::false_type) { return false; }

public:
	static constexpr bool value = ( is_group_abelian(
	std::bool_constant<group<DispatchTag, impl::underlying_group_of_ring_t<DispatchTag, StructureTag>>::value>{} ) &&
									!(std::is_base_of_v<impl::no_impl, impl::is_abelian<DispatchTag, StructureTag>> ||
									 std::is_base_of_v<impl::no_impl, impl::multiply_assign<DispatchTag, StructureTag>> ||
									 std::is_base_of_v<impl::no_impl, impl::one<DispatchTag, StructureTag>>));
};
}
}

TAGD_CONCEPT_IMPLEMENTS_FUNCTION( ring, void, is_abelian )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( ring, void, add_assign )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( ring, void, add )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( ring, void, subtract_assign )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( ring, void, subtract )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( ring, void, multiply_assign )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( ring, void, multiply )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( ring, void, negate_in_place )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( ring, void, negate )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( ring, void, zero )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( ring, void, one )

TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( add_assign )
TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( add )
TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( subtract_assign )
TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( subtract )
TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( multiply_assign )
TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( multiply )
TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( negate_in_place )
TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( negate )
TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(zero)
TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(one)

#endif //TAGD_CONCEPTS_RING_HPP
