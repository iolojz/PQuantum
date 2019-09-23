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


#ifdef TAGD_ENABLE_IF_TAG_IS
#error "TAGD_ENABLE_IF_TAG_IS is already defined."
#endif
#define TAGD_ENABLE_IF_TAG_IS( type, tag ) \
class = std::enable_if_t<std::is_same_v<::tag_dispatch::tag_of_t<type>, tag>>

#ifdef TAGD_DISABLE_IF_TAG_IS
#error "TAGD_DISABLE_IF_TAG_IS is already defined."
#endif
#define TAGD_DISABLE_IF_TAG_IS( type, tag ) \
class = std::enable_if_t<!std::is_same_v<::tag_dispatch::tag_of_t<type>, tag>>


template<template<class, class> class Implementation>
struct impl_tag {
};

template<class StructureTag, class DispatchTag, template<class, class> class Implementation>
static constexpr StructureTag default_structure_tag( DispatchTag, impl_tag<Implementation> );

template<class DispatchTag, class StructureTag, template<class, class> class Implementation>
struct infer_tags
{
	using dispatch_tag = DispatchTag;
	using structure_tag = StructureTag;
};

template<class DispatchTag, template<class, class> class Implementation>
struct infer_tags<DispatchTag, void, Implementation>
{
	using dispatch_tag = DispatchTag;
	using structure_tag = std::decay_t<decltype( default_structure_tag( std::declval<dispatch_tag>(),
																		std::declval<impl_tag<Implementation>>()))>;
};

namespace impl
{
template<class ToTag, class FromTag, class StructureTag>
struct to : no_impl
{
};
}

template<class ToTag, class StructureTag = void> static constexpr auto to = []( auto &&arg )
constexpr -> decltype( auto )
{
using arg_type = decltype( arg );
return
impl::to<ToTag, tag_of_t<arg_type>, StructureTag>::apply( std::forward<arg_type>( arg )
);
};

namespace detail {
template<class ToTag, class StructureTag> static constexpr decltype( auto ) forward_or_convert = []( auto &&arg )
constexpr -> decltype( auto )
{
using arg_type = decltype( arg );
using tag = tag_of_t<arg_type>;

if constexpr(std::is_same_v<tag, ToTag>)
return
std::forward<arg_type>( arg );
else
return
to<ToTag, StructureTag>( std::forward<arg_type>( arg )
);
};

template<class DispatchTag, class StructureTag, template<class, class> class Implementation>
struct function_object {
	template<class ...Args>
	decltype(auto) constexpr operator()(Args &&... args) const {
		return Implementation<DispatchTag, StructureTag>::apply( std::forward<Args>( args )... );
	}
};

template<class DispatchTag, template<class, class> class Implementation>
struct function_object<DispatchTag, void, Implementation> {
	template<class ...Args>
	decltype(auto) constexpr operator()(Args &&... args) const {
		using structure_tag = std::decay_t<decltype( default_structure_tag( std::declval<DispatchTag>(), std::declval<
		impl_tag < Implementation>>()))>;
		return Implementation<DispatchTag, structure_tag>::apply( std::forward<Args>( args )... );
	}
};

template<class StructureTag, class ConversionTag, template<class, class> class Implementation>
struct tag_inferring_function_object {
	template<class ...Args>
	decltype(auto) constexpr operator()(Args &&... args) const {
		using tag_inferrer = infer_tags<std::common_type_t<tag_of_t < Args>...>, StructureTag, Implementation >;
		using dispatch_tag = typename tag_inferrer::dispatch_tag;
		using structure_tag = typename tag_inferrer::structure_tag;
		
		return Implementation<dispatch_tag, structure_tag>::apply(
		forward_or_convert<dispatch_tag, ConversionTag>( std::forward<Args>( args ))... );
	}
};
}

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
template<class StructureTag = void, class ConversionTag = void> static constexpr detail::tag_inferring_function_object<StructureTag, ConversionTag, impl::function_name> function_name; \
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
> default_structure_tag( DispatchTag, impl_tag<impl::function> ) { return {}; } \
}

#ifdef TAGD_MODELLED_CONCEPT_IMPLEMENTS_FUNCTION
#error "TAGD_MODELLED_CONCEPT_IMPLEMENTS_FUNCTION is already defined."
#endif
#define TAGD_MODELLED_CONCEPT_IMPLEMENTS_FUNCTION( dispatch_tag, concept, function ) \
namespace tag_dispatch \
{ \
static constexpr concepts::concept<dispatch_tag, void> \
default_structure_tag( dispatch_tag, impl_tag<impl::function> ) { return {}; } \
}

#include "concepts/makeable.hpp"

namespace tag_dispatch::impl
{
template<class ToTag, class StructureTag>
struct to<ToTag, ToTag, StructureTag>
{
	template<class Arg>
	static constexpr decltype( auto ) apply( Arg &&arg )
	{
		constexpr auto make_copy = tag_dispatch::make<ToTag, rebind_concept_t<concepts::makeable, ToTag>>;
		auto copy = make_copy( std::forward<Arg>( arg ));
		return std::forward<Arg>( arg );
	}
};
}

#include "concepts/boolean_lattice.hpp"
#include "concepts/group.hpp"
#include "concepts/monoid.hpp"
#include "concepts/ring.hpp"
#include "concepts/set.hpp"
#include "concepts/total_order.hpp"
#include "concepts/total_preorder.hpp"

#include "models/bool.hpp"
#include "models/free_monomial.hpp"
#include "models/free_polynomial.hpp"
#include "models/int.hpp"
#include "models/quotient.hpp"

#endif //TAGD_TAG_DISPATCH_HPP
