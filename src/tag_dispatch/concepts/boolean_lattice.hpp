//
// Created by jayz on 29.08.19.
//

#ifndef TAG_DISPATCH_CONCEPTS_BOOLEAN_LATTICE_HPP
#define TAG_DISPATCH_CONCEPTS_BOOLEAN_LATTICE_HPP

#include "tag_dispatch/tag_dispatch.hpp"

namespace tag_dispatch {
namespace impl {
template<class DispatchTag, class StructureTag>
struct not_ : no_impl {
};

template<class DispatchTag, class StructureTag>
struct and_ : no_impl {
};

template<class DispatchTag, class StructureTag>
struct or_ : no_impl {
};
}

namespace concepts {
template<class DispatchTag, class StructureTag>
struct boolean_lattice;
template<class DispatchTag, class StructureTag = boolean_lattice<DispatchTag, void>>
struct boolean_lattice {
	static constexpr bool value = !(std::is_base_of_v<impl::no_impl, impl::not_<DispatchTag, StructureTag>> ||
									std::is_base_of_v<impl::no_impl, impl::and_<DispatchTag, StructureTag>> ||
									std::is_base_of_v<impl::no_impl, impl::or_<DispatchTag, StructureTag>>);
};
}

template<class DispatchTag> static constexpr std::enable_if_t<concepts::boolean_lattice<DispatchTag>::value, concepts::boolean_lattice<DispatchTag, void>> default_structure_tag(
		impl_tag < impl::not_ > );
template<class DispatchTag> static constexpr std::enable_if_t<concepts::boolean_lattice<DispatchTag>::value, concepts::boolean_lattice<DispatchTag, void>> default_structure_tag(
		impl_tag < impl::and_ > );
template<class DispatchTag> static constexpr std::enable_if_t<concepts::boolean_lattice<DispatchTag>::value, concepts::boolean_lattice<DispatchTag, void>> default_structure_tag(
		impl_tag < impl::or_ > );
}

TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(not_)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(and_)
TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(or_)

#endif // TAG_DISPATCH_CONCEPTS_BOOLEAN_LATTICE_HPP
