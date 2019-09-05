//
// Created by jayz on 03.09.19.
//

#ifndef PQUANTUM_TAG_DISPATCH_COMPARABLE_HPP
#define PQUANTUM_TAG_DISPATCH_COMPARABLE_HPP

#include "tag_dispatch/tag_dispatch.hpp"

#include <functional>

namespace PQuantum::tag_dispatch {
namespace impl {
template<class Tag>
struct equal : std::false_type {
};

template<class Tag>
struct not_equal {
	template<class FunctionTag = void, class ...Args>
	static decltype(auto) apply(Args &&...args) {
		return equal<Tag>::template apply<FunctionTag>(std::forward<Args>(args)...);
	}
};
}

namespace concepts {
template<class Tag>
struct comparable {
	static constexpr bool value = std::conditional_t<synthesized<comparable, Tag>::value, true, impl::equal<Tag>::value>;
};
}
}

PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(equal, comparable)
PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION( unequal )

#endif //PQUANTUM_TAG_DISPATCH_COMPARABLE_HPP
