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
	template<class ...Args>
	static decltype(auto) apply(Args &&...args) {
		return tag_dispatch::not_<>(less_equal<DispatchTag, StructureTag>::apply(std::forward<Args>(args)...));
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
		impl_tag < impl::less_equal > );

template<class DispatchTag> static constexpr decltype(default_structure_tag<DispatchTag>(
		std::declval<impl_tag < impl::less_equal>>()))
default_structure_tag(impl_tag<impl::greater>);
}

TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(less_equal)
TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(greater)

#endif // TAG_DISPATCH_CONCEPTS_TOTAL_PREORDER_HPP
