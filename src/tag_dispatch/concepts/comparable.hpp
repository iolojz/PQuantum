//
// Created by jayz on 03.09.19.
//

#ifndef PQUANTUM_TAG_DISPATCH_COMPARABLE_HPP
#define PQUANTUM_TAG_DISPATCH_COMPARABLE_HPP

#include "tag_dispatch/tag_dispatch.hpp"

#include <functional>

PQUANTUM_DEFINE_DEFAULT_TAG_DISPATCHED_FUNCTION(equal)

namespace PQuantum::tag_dispatch {
namespace impl {
template<class Tag>
struct not_equal {
	template<class FunctionTag = void, class ...Args>
	static decltype(auto) apply(Args &&...args) {
		// FIXME: should be not
		return !equal<Tag>::template apply<FunctionTag>(std::forward<Args>(args)...);
	}
};
}

namespace concepts {
template<class Tag>
struct comparable {
	static constexpr bool value = std::is_base_of_v<impl::detail::no_impl, impl::equal<Tag>>;
};
}
}

PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(equal, comparable)
		PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(not_equal)

#endif //PQUANTUM_TAG_DISPATCH_COMPARABLE_HPP
