//
// Created by jayz on 29.08.19.
//

#ifndef TAG_DISPATCH_CONCEPTS_TOTAL_PREORDER_HPP
#define TAG_DISPATCH_CONCEPTS_TOTAL_PREORDER_HPP

#include "total_preorder.hpp"

namespace tag_dispatch {
namespace impl {
template<class DispatchTag, class StructureTag>
struct less_equal : no_impl {
};

template<class DispatchTag, class StructureTag>
struct greater {
	template<class Arg1, class Arg2>
	static decltype( auto ) apply( Arg1 &&arg1, Arg2 &&arg2 )
	{
		return tag_dispatch::not_<>(
		less_equal<DispatchTag, StructureTag>::apply( std::forward<Arg1>( arg1 ), std::forward<Arg2>( arg2 )));
	}
};
}

namespace concepts {
template<class DispatchTag, class StructureTag>
struct total_preorder;
template<class DispatchTag, class StructureTag = total_preorder<DispatchTag, void>>
struct total_preorder {
	static constexpr bool value = !std::is_base_of_v<impl::no_impl, impl::less_equal<DispatchTag, StructureTag>>;
};
}

template<class DispatchTag> static constexpr std::enable_if_t<concepts::total_preorder<DispatchTag>::value, concepts::total_preorder<DispatchTag, void>> default_structure_tag(
impl_tag <impl::less_equal> )
{
	return {};
}

template<class DispatchTag>
static constexpr auto default_structure_tag( impl_tag <impl::greater> )
{
	return default_structure_tag<DispatchTag>( impl_tag < impl::less_equal > {} );
}
}

TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(less_equal)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(greater)

#endif // TAG_DISPATCH_CONCEPTS_TOTAL_PREORDER_HPP
