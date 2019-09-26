//
// Created by jayz on 29.08.19.
//

#ifndef TAG_DISPATCH_CONCEPTS_TOTAL_PREORDER_HPP
#define TAG_DISPATCH_CONCEPTS_TOTAL_PREORDER_HPP

#include "set.hpp"
#include "tag_dispatch/models/quotient.hpp"

namespace tag_dispatch {
namespace concepts {
template<class DispatchTag, class StructureTag>
struct total_order;

template<class DispatchTag, class StructureTag>
struct total_preorder;
}

namespace impl {
template<class DispatchTag, class StructureTag>
struct less_equal : no_impl {
};

template<class DispatchTag, class StructureTag>
struct greater {
	template<class Arg1, class Arg2>
	static decltype(auto) apply(Arg1 &&arg1, Arg2 &&arg2) {
		return tag_dispatch::not_<>(
				less_equal<DispatchTag, StructureTag>::apply(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2)));
	}
};

template<class DispatchTag, class StructureTag>
struct less;

template<class DispatchTag, class StructureTag>
struct less<DispatchTag, models::quotient_tag<DispatchTag, concepts::total_preorder<DispatchTag, StructureTag>>>
{
template<class Arg1, class Arg2>
static constexpr decltype(auto) apply( const Arg1 &arg1, const Arg2 &arg2) {
	using less_equal_ = less_equal<DispatchTag, concepts::total_preorder<DispatchTag, StructureTag>>;
	return tag_dispatch::and_<>( less_equal_::apply( arg1, arg2 ),
								 tag_dispatch::not_<>( less_equal_::apply( arg2, arg1 )));
}
};
}

namespace concepts {
template<class DispatchTag, class StructureTag = total_preorder<DispatchTag, void>>
struct total_preorder {
	static constexpr bool value = !std::is_base_of_v<impl::no_impl, impl::less_equal<DispatchTag, StructureTag>>;
};
}
}

TAGD_CONCEPT_IMPLEMENTS_FUNCTION( total_preorder, void, less_equal )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( total_preorder, void, greater )

TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( less_equal )
TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( greater )

#endif // TAG_DISPATCH_CONCEPTS_TOTAL_PREORDER_HPP
