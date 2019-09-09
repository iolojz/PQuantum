//
// Created by jayz on 03.09.19.
//

#ifndef TAGD_TAG_DISPATCH_HPP
#define TAGD_TAG_DISPATCH_HPP

#include <type_traits>

#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/pop_front.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>

namespace tag_dispatch {
namespace detail {
template<class, class = std::void_t<>>
struct has_dispatch_tag : std::false_type {
};

template<class T>
struct has_dispatch_tag<T, std::void_t<typename T::dispatch_tag>> : std::true_type {
};

template<class T, bool>
struct tag_of;

template<class T>
struct tag_of<T, true> {
	using type = typename T::dispatch_tag;
};

template<class T>
struct tag_of<T, false> {
	using type = std::decay_t<T>;
};
}

namespace impl {
template<class T>
struct tag_of {
	using type = typename detail::tag_of<T, detail::has_dispatch_tag<T>::value>::type;
};

struct no_impl {
};
}

template<class T>
struct tag_of {
	using type = typename impl::tag_of<std::decay_t<T>>::type;
};
template<class T> using tag_of_t = typename tag_of<T>::type;

template<template<class, class> class Implementation>
struct impl_tag {
};

template<class DispatchTag, class StructureTag, template<class, class> class Implementation>
static constexpr StructureTag default_structure_tag(impl_tag<Implementation>);

namespace detail {
template<class StructureTag, template<class, class> class Implementation>
struct function_object {
	template<class ...Args>
	decltype(auto) operator()(Args &&... args) const {
		using dispatch_tag = tag_of_t<std::common_type_t<Args...>>;
		return Implementation<dispatch_tag, StructureTag>::apply(std::forward<Args>(args)...);
	}
};

template<template<class, class> class Implementation>
struct function_object<void, Implementation> {
	template<class ...Args>
	constexpr decltype(auto) operator()(Args &&... args) const {
		using dispatch_tag = std::common_type_t<tag_of_t<Args>...>;
		static_assert(
				std::is_void_v<std::void_t<decltype(default_structure_tag<dispatch_tag>(impl_tag<Implementation>{}))>>,
				"Call to tag-dispatched function \"equal\" is ambiguous: Cannot determine structure tag.");
		using structure_tag = std::decay_t<decltype(default_structure_tag<dispatch_tag>(impl_tag<Implementation>{}))>;
		
		return Implementation<dispatch_tag, structure_tag>::apply(std::forward<Args>(args)...);
	}
};
}
}

#ifdef TAGD_DEFINE_TAG_DISPATCHED_FUNCTION
#error "TAGD_DEFINE_TAG_DISPATCHED_FUNCTION is already defined."
#endif
#define TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(function_name) \
namespace tag_dispatch \
{ \
template<class StructureTag = void> static constexpr detail::function_object<StructureTag, impl::function_name> function_name; \
}

#ifdef TAGD_ENABLE_IF_TAG_IS
#error "TAGD_ENABLE_IF_TAG_IS is already defined."
#endif
#define TAGD_ENABLE_IF_TAG_IS(type, tag) \
class = std::enable_if_t<std::is_same_v<PQuantum::tag_dispatch::tag_of_t<type>, tag>>

#endif //TAGD_TAG_DISPATCH_HPP
