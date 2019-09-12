//
// Created by jayz on 29.08.19.
//

#ifndef TAG_DISPATCH_CONCEPTS_SET_HPP
#define TAG_DISPATCH_CONCEPTS_SET_HPP

#include "boolean_lattice.hpp"

namespace tag_dispatch {
namespace impl {
template<class DispatchTag, class StructureTag>
struct underlying_set;
template<class DispatchTag, class StructureTag> using underlying_set_t = typename underlying_set<DispatchTag, StructureTag>::type;

template<class DispatchTag, class StructureTag>
struct equal : no_impl {
};

template<class DispatchTag, class StructureTag>
struct not_equal {
	template<class ...Args>
	static decltype(auto) apply(Args &&...args) {
		return tag_dispatch::not_<>(equal<DispatchTag, StructureTag>::apply(std::forward<Args>(args)...));
	}
};
}

namespace concepts {
template<class DispatchTag, class StructureTag>
struct set;
template<class DispatchTag, class StructureTag = set<DispatchTag, void>>
struct set {
	static constexpr bool value = !std::is_base_of_v<impl::no_impl, impl::equal<DispatchTag, StructureTag>>;
};
}

template<class DispatchTag> static constexpr std::enable_if_t<concepts::set<DispatchTag>::value, concepts::set<DispatchTag, void>> default_structure_tag(
impl_tag <impl::equal> )
{
	return {};
}

template<class DispatchTag>
static constexpr auto default_structure_tag( impl_tag <impl::not_equal> )
{
	return default_structure_tag<DispatchTag>( impl_tag < impl::equal > {} );
}
}

TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(equal)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(not_equal)

#endif // TAG_DISPATCH_CONCEPTS_SET_HPP
