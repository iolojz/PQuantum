//
// Created by jayz on 29.08.19.
//

#ifndef TAG_DISPATCH_CONCEPTS_TOTAL_ORDER_HPP
#define TAG_DISPATCH_CONCEPTS_TOTAL_ORDER_HPP

#include "set.hpp"
#include "total_preorder.hpp"

namespace tag_dispatch {
namespace impl {
template<class DispatchTag, class StructureTag>
struct less : no_impl {
};

template<class DispatchTag, class StructureTag>
struct greater_equal {
	template<class Arg1, class Arg2>
	static decltype(auto) apply(Arg1 &&arg1, Arg2 &&arg2) {
		return tag_dispatch::not_<>(
				less<DispatchTag, StructureTag>::apply(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2)));
	}
};
}

namespace concepts {
template<class DispatchTag, class StructureTag>
struct total_order;
template<class DispatchTag, class StructureTag = total_order<DispatchTag, void>>
struct total_order {
	static constexpr bool value = !std::is_base_of_v<impl::no_impl, impl::less<DispatchTag, StructureTag>>;
};

template<class DispatchTag>
struct total_preorder<DispatchTag, total_order<DispatchTag, void>> {
	static constexpr bool value = total_order<DispatchTag, total_order<DispatchTag, void>>::value;
};

template<class DispatchTag>
struct set<DispatchTag, total_order<DispatchTag, void>> {
	static constexpr bool value = total_order<DispatchTag, total_order<DispatchTag, void>>::value;
};
}

namespace impl {
template<class DispatchTag>
struct equal<DispatchTag, concepts::total_order<DispatchTag, void>> {
private:
	using structure_tag = concepts::total_order<DispatchTag, void>;
public:
	template<class Arg1, class Arg2>
	static decltype(auto) apply(const Arg1 &arg1, const Arg2 &arg2) {
		return tag_dispatch::and_<>(less<DispatchTag, structure_tag>::apply(arg1, arg2),
									less<DispatchTag, structure_tag>::apply(arg2, arg1));
	}
};

template<class DispatchTag>
struct less_equal<DispatchTag, concepts::total_order<DispatchTag, void>> {
private:
	using structure_tag = concepts::total_order<DispatchTag, void>;
public:
	
	template<class Arg1, class Arg2>
	static decltype(auto) apply(const Arg1 &arg1, const Arg2 &arg2) {
		return tag_dispatch::or_<>(less<DispatchTag, structure_tag>::apply(arg1, arg2),
								   equal<DispatchTag, structure_tag>::apply(arg1, arg2));
	}
};
}

template<class DispatchTag>
static constexpr std::enable_if_t<concepts::total_order<DispatchTag>::value, concepts::total_order<DispatchTag, void>>
default_structure_tag(impl_tag<impl::less>);

template<class DispatchTag>
static constexpr decltype(default_structure_tag<DispatchTag>(std::declval<impl_tag<impl::less>>()))
default_structure_tag(impl_tag<impl::greater_equal>);

template<class DispatchTag>
static constexpr std::enable_if_t<concepts::total_order<DispatchTag>::value, concepts::total_order<DispatchTag, void>>
default_structure_tag(impl_tag<impl::less_equal>);

template<class DispatchTag>
static constexpr std::enable_if_t<concepts::total_order<DispatchTag>::value, concepts::total_order<DispatchTag, void>>
default_structure_tag(impl_tag<impl::equal>);
}

TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(less)
TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(greater_equal)

#endif // TAG_DISPATCH_CONCEPTS_TOTAL_ORDER_HPP
