//
// Created by jayz on 03.09.19.
//

#ifndef PQUANTUM_TAG_DISPATCH_ORDERABLE_HPP
#define PQUANTUM_TAG_DISPATCH_ORDERABLE_HPP

#include "tag_dispatch/tag_dispatch.hpp"
#include "comparable.hpp"

#include <functional>

PQUANTUM_DEFINE_DEFAULT_TAG_DISPATCHED_FUNCTION(less)

namespace PQuantum::tag_dispatch {
namespace concepts {
template<class Tag>
struct orderable {
	static constexpr bool value = std::is_base_of_v<impl::detail::no_impl, impl::less<Tag>>;
};
}

namespace synthesized {
// FIXME: This will not work for multiple concepts synthesizing things...
template<class Tag, class Arg1, class Arg2, PQUANTUM_ENABLE_IF_TAG_IMPLEMENTS_CONCEPT(Tag, concepts::orderable) >
static decltype(auto) equal(const Arg1 &arg1, const Arg2 &arg2) {
	// FIXME: should be "not"
	return !(less(arg1, arg2) || less(arg2, arg1));
}
}
}

PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(less)
		
		PQUANTUM_TAG_DISPATCH_CONCEPT_REFINES_CONCEPT(orderable, comparable)

/*
PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(less_equal)
PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(greater_equal)
PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(greater)

PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(equal, comparable)
PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(unequal)*/

#endif //PQUANTUM_TAG_DISPATCH_COMPARABLE_HPP
