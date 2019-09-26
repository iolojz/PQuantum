//
// Created by jayz on 03.09.19.
//

#ifndef PQUANTUM_MAKE_HPP
#define PQUANTUM_MAKE_HPP

#include "tag_dispatch/tag_dispatch.hpp"

namespace tag_dispatch {
namespace impl {
template<class DispatchTag, class StructureTag>
struct make : no_impl {
};
}

namespace concepts {
template<class DispatchTag, class StructureTag>
struct makeable;
template<class DispatchTag, class StructureTag = makeable<DispatchTag, void>>
struct makeable {
	static constexpr bool value = !std::is_base_of_v<impl::no_impl, impl::make<DispatchTag, StructureTag>>;
};
}
}

TAGD_CONCEPT_IMPLEMENTS_FUNCTION( makeable, void, make )

TAGD_DEFINE_TAG_DISPATCHED_FUNCTION( make )

#endif //PQUANTUM_MAKE_HPP
