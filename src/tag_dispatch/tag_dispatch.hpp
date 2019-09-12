//
// Created by jayz on 03.09.19.
//

#ifndef TAGD_TAG_DISPATCH_HPP
#define TAGD_TAG_DISPATCH_HPP

#include <type_traits>

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
template<class DispatchTag, class StructureTag, template<class, class> class Implementation>
struct function_object {
	template<class ...Args>
	decltype(auto) constexpr operator()(Args &&... args) const {
		return Implementation<DispatchTag, StructureTag>::apply(std::forward<Args>(args)...);
	}
};

template<class DispatchTag, template<class, class> class Implementation>
struct function_object<DispatchTag, void, Implementation> {
	template<class ...Args>
	decltype(auto) constexpr operator()(Args &&... args) const {
		using structure_tag = std::decay_t<decltype(default_structure_tag<DispatchTag>(impl_tag<Implementation>{}))>;
		return Implementation<DispatchTag, structure_tag>::apply(std::forward<Args>(args)...);
	}
};

template<class StructureTag, template<class, class> class Implementation>
struct tag_inferring_function_object {
	template<class ...Args>
	decltype(auto) constexpr operator()(Args &&... args) const {
		using dispatch_tag = tag_of_t<std::common_type_t<Args...>>;
		return Implementation<dispatch_tag, StructureTag>::apply(std::forward<Args>(args)...);
	}
};

template<template<class, class> class Implementation>
struct tag_inferring_function_object<void, Implementation> {
	template<class ...Args>
	constexpr decltype(auto) operator()(Args &&... args) const {
		using dispatch_tag = std::common_type_t<tag_of_t<Args>...>;
		using structure_tag = std::decay_t<decltype(default_structure_tag<dispatch_tag>(impl_tag<Implementation>{}))>;
		
		return Implementation<dispatch_tag, structure_tag>::apply(std::forward<Args>(args)...);
	}
};
}

template<class DispatchTag, class StructureTag>
struct quotient_tag;

template<template<class, class> class Concept, class StructureTag>
struct rebind_concept;

template<template<class, class> class NewConcept,
		template<class, class> class OldConcept, class DispatchTag, class StructureTag>
struct rebind_concept<NewConcept, OldConcept<DispatchTag, StructureTag>> {
	using type = NewConcept<DispatchTag, StructureTag>;
};

template<template<class, class> class Concept, class StructureTag> using rebind_concept_t = typename rebind_concept<Concept, StructureTag>::type;
}

#ifdef TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION
#error "TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION is already defined."
#endif
#define TAGD_DEFINE_TAG_INFERRING_DISPATCHED_FUNCTION(function_name) \
namespace tag_dispatch \
{ \
template<class StructureTag = void> static constexpr detail::tag_inferring_function_object<StructureTag, impl::function_name> function_name; \
}

#ifdef TAGD_DEFINE_TAG_DISPATCHED_FUNCTION
#error "TAGD_DEFINE_TAG_DISPATCHED_FUNCTION is already defined."
#endif
#define TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(function_name) \
namespace tag_dispatch \
{ \
template<class DispatchTag, class StructureTag = void> static constexpr detail::function_object<DispatchTag, StructureTag, impl::function_name> function_name; \
}

#ifdef TAGD_CONCEPT_IMPLEMENTS_FUNCTION
#error "TAGD_CONCEPT_IMPLEMENTS_FUNCTION is already defined."
#endif
#define TAGD_CONCEPT_IMPLEMENTS_FUNCTION(concept, function) \
namespace tag_dispatch \
{ \
template<class DispatchTag> static constexpr std::enable_if_t< \
    concepts::concept<DispatchTag>::value, \
    concepts::concept<DispatchTag, void> \
> default_structure_tag( impl_tag<impl::function> ) { return {}; } \
}

#ifdef TAGD_ENABLE_IF_TAG_IS
#error "TAGD_ENABLE_IF_TAG_IS is already defined."
#endif
#define TAGD_ENABLE_IF_TAG_IS(type, tag) \
class = std::enable_if_t<std::is_same_v<::tag_dispatch::tag_of_t<type>, tag>>

#ifdef TAGD_DISABLE_IF_TAG_IS
#error "TAGD_DISABLE_IF_TAG_IS is already defined."
#endif
#define TAGD_DISABLE_IF_TAG_IS(type, tag) \
class = std::enable_if_t<!std::is_same_v<::tag_dispatch::tag_of_t<type>, tag>>

#include "concepts/boolean_lattice.hpp"
#include "concepts/group.hpp"
#include "concepts/makeable.hpp"
#include "concepts/monoid.hpp"
#include "concepts/ring.hpp"
#include "concepts/set.hpp"
#include "concepts/total_order.hpp"
#include "concepts/total_preorder.hpp"

#include "models/bool.hpp"
#include "models/int.hpp"
#include "models/free_monomial.hpp"
#include "models/free_polynomial.hpp"

#endif //TAGD_TAG_DISPATCH_HPP
