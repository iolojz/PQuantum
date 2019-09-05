//
// Created by jayz on 03.09.19.
//

#ifndef PQUANTUM_TAG_DISPATCH_ORDERABLE_HPP
#define PQUANTUM_TAG_DISPATCH_ORDERABLE_HPP

#include "tag_dispatch/tag_dispatch.hpp"
#include "comparable.hpp"

#include <functional>

namespace PQuantum::tag_dispatch {
namespace impl {
template<class Tag>
struct less : std::false_type {
};

template<class Tag>
struct equal : std::false_type {
};
template<class Tag>
struct less : std::false_type {
};
}

namespace concepts {
template<class Tag>
struct orderable {
	static constexpr bool value = impl::less::value;
};

template<class Tag>
struct is_synthesized {
	template<>
	static constexpr bool value(void) { return false; }
};
}
}

namespace impl {
#ifdef PQUANTUM_DEFAULT_IMPL_TAG_DISPATCHED_COMPARISON
#error "PQUANTUM_DEFAULT_IMPL_TAG_DISPATCHED_COMPARISON is already defined."
#endif

#define PQUANTUM_DEFAULT_IMPL_TAG_DISPATCHED_COMPARISON(name, std_name) \
template<class Tag> \
struct name \
{ \
    template<class Arg1, class Arg2> static decltype(auto) apply( Arg1 &&arg1, Arg2 &&arg2 ) \
    { return std::std_name<>{}( std::forward<Arg1>( arg1 ), std::forward<Arg2>( arg2 ) ); } \
}

PQUANTUM_DEFAULT_IMPL_TAG_DISPATCHED_COMPARISON(less, less);

PQUANTUM_DEFAULT_IMPL_TAG_DISPATCHED_COMPARISON(less_equal, less_equal);

PQUANTUM_DEFAULT_IMPL_TAG_DISPATCHED_COMPARISON(unequal, not_equal_to);

PQUANTUM_DEFAULT_IMPL_TAG_DISPATCHED_COMPARISON(equal, equal_to);

PQUANTUM_DEFAULT_IMPL_TAG_DISPATCHED_COMPARISON(greater_equal, greater_equal);

PQUANTUM_DEFAULT_IMPL_TAG_DISPATCHED_COMPARISON(greater, greater);

#undef PQUANTUM_DEFAULT_IMPL_TAG_DISPATCHED_COMPARISON
}
}

PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(less)
PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(less_equal)
PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(greater_equal)
PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(greater)

PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(equal, comparable)
PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(unequal)

#endif //PQUANTUM_TAG_DISPATCH_COMPARABLE_HPP
