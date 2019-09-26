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
}

namespace impl {
template<class DispatchTag, class StructureTag>
struct equal<DispatchTag, concepts::total_order<DispatchTag, StructureTag>>
{
private:
	using less_ = less<DispatchTag, concepts::total_order<DispatchTag, StructureTag>>;
public:
	template<class Arg1, class Arg2>
	static decltype( auto ) constexpr apply( const Arg1 &arg1, const Arg2 &arg2 )
	{
		return tag_dispatch::and_<>( tag_dispatch::not_<>( less_::apply( arg1, arg2 )),
									 tag_dispatch::not_<>( less_::apply( arg2, arg1 )));
	}
};

template<class DispatchTag, class StructureTag>
struct less_equal<DispatchTag, concepts::total_order<DispatchTag, StructureTag>>
{
private:
	using less_ = less<DispatchTag, concepts::total_order<DispatchTag, StructureTag>>;
public:
	template<class Arg1, class Arg2>
	static decltype(auto) apply(const Arg1 &arg1, const Arg2 &arg2) {
		return tag_dispatch::not_<>( less_::apply( arg2, arg1 ));
	}
};
}
}

TAGD_CONCEPT_IMPLEMENTS_FUNCTION( total_order, void, less )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( total_order, void, less_equal )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( total_order, void, equal )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( total_order, void, greater_equal )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( total_order, void, greater )
TAGD_CONCEPT_IMPLEMENTS_FUNCTION( total_order, void, not_equal )

TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( less )
TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( greater_equal )

#endif // TAG_DISPATCH_CONCEPTS_TOTAL_ORDER_HPP
