//
// Created by jayz on 24.09.19.
//

#ifndef TAGD_CORE_FUNCTION_DISPATCH_HPP
#define TAGD_CORE_FUNCTION_DISPATCH_HPP

#include "dispatch_tags.hpp"

namespace tag_dispatch
{
namespace impl
{
struct no_impl
{
};
template<class ToTag, class FromTag, class StructureTag>
struct to : no_impl
{
};
}

template<class ToTag, class StructureTag = void> static constexpr auto to = []( auto &&arg )
constexpr -> decltype( auto )
{
using arg_type = decltype( arg );
static_assert( !std::is_base_of_v<impl::no_impl, impl::to<ToTag, tag_of_t<arg_type>, StructureTag>>,
			   "Unknown conversion" );
return
impl::to<ToTag, tag_of_t<arg_type>, StructureTag>::apply( std::forward<arg_type>( arg )
);
};

namespace impl
{
template<class ToTag, class StructureTag = void>
struct forward_or_convert
{
	template<class Arg>
	static constexpr decltype( auto ) apply( Arg &&arg )
	{
		using from_tag = tag_of_t<Arg>;
		
		if constexpr( std::is_same_v<from_tag, ToTag> )
			return std::forward<Arg>( arg );
		else
			return tag_dispatch::to<ToTag, StructureTag>( std::forward<Arg>( arg ));
	}
};
}

template<class ToTag, class StructureTag = void> static constexpr auto forward_or_convert = []( auto &&arg )
constexpr -> decltype( auto ) {
using arg_type = decltype( arg );
return impl::to<ToTag, tag_of_t < arg_type>, StructureTag>
::apply( std::forward<arg_type>( arg )
);
};

template<template<class, class> class Implementation>
struct impl_tag
{
};

template<class StructureTag, class DispatchTag, template<class, class> class Implementation>
static constexpr StructureTag default_structure_tag( DispatchTag, impl_tag<Implementation> );

template<class DispatchTag, template<class, class> class Implementation>
struct structure_tag_for_function_dispatch
{
	using type = std::decay_t<decltype( default_structure_tag( std::declval<DispatchTag>(),
															   std::declval<impl_tag<Implementation>>()))>;
};

template<template<class, class> class Implementation, class ...Args>
struct tags_for_function_dispatch
{
	using dispatch_tag = common_tag_t<tag_of_t <Args>...>;
	using structure_tag = typename structure_tag_for_function_dispatch<dispatch_tag, Implementation>::type;
};

struct nop_transformation
{
};

template<class StructureTag, template<class, class> class Implementation, class ...Args>
struct function_traits_for_dispatch
{
	using dispatch_tag = common_tag_t<tag_of_t <Args>...>;
	using structure_tag = StructureTag;
	using pre_action = impl::forward_or_convert<dispatch_tag>;
	using post_action = nop_transformation;
};

namespace detail
{
template<class DispatchTag, class StructureTag, template<class, class> class Implementation>
struct function_object
{
	template<class ...Args>
	decltype( auto ) constexpr operator()( Args &&... args ) const
	{
		return Implementation<DispatchTag, StructureTag>::apply( std::forward<Args>( args )... );
	}
};

template<class DispatchTag, template<class, class> class Implementation>
struct function_object<DispatchTag, void, Implementation>
{
	template<class ...Args>
	decltype( auto ) constexpr operator()( Args &&... args ) const
	{
		using structure_tag = typename structure_tag_for_function_dispatch<DispatchTag, Implementation>::type;
		return Implementation<DispatchTag, structure_tag>::apply( std::forward<Args>( args )... );
	}
};

template<class StructureTag, template<class, class> class Implementation>
struct traited_function_object
{
	template<class ...Args>
	decltype( auto ) constexpr operator()( Args &&... args ) const
	{
		using function_traits = function_traits_for_dispatch<StructureTag, Implementation, Args...>;
		using dispatch_tag = typename function_traits::dispatch_tag;
		using structure_tag = typename function_traits::structure_tag;
		using pre_action = typename function_traits::pre_action;
		using post_action = typename function_traits::post_action;
		
		if constexpr( std::is_same_v<pre_action, nop_transformation> ) {
			if constexpr( std::is_same_v<post_action, nop_transformation> )
				return Implementation<dispatch_tag, structure_tag>::apply( std::forward<Args>( args )... );
			else
				return post_action::apply(
				Implementation<dispatch_tag, structure_tag>::apply( std::forward<Args>( args )... ));
		} else if constexpr( std::is_same_v<post_action, nop_transformation> )
			return Implementation<dispatch_tag, structure_tag>::apply(
			pre_action::apply( std::forward<Args>( args ))... );
		else
			return post_action::apply(
			Implementation<dispatch_tag, structure_tag>::apply( pre_action::apply( std::forward<Args>( args ))... ));
	}
};

template<template<class, class> class Implementation>
struct traited_function_object<void, Implementation>
{
	template<class ...Args>
	decltype( auto ) constexpr operator()( Args &&... args ) const
	{
		using preliminary_structure_tag = typename tags_for_function_dispatch<Implementation, Args...>::structure_tag;
		return traited_function_object<preliminary_structure_tag, Implementation>{}( std::forward<Args>( args )... );
	}
};
}

template<template<class, class> class Concept, class StructureTag>
struct rebind_concept;

template<template<class, class> class NewConcept,
template<class, class> class OldConcept, class DispatchTag, class StructureTag>
struct rebind_concept<NewConcept, OldConcept<DispatchTag, StructureTag>>
{
	using type = NewConcept<DispatchTag, StructureTag>;
};
template<template<class, class> class Concept, class StructureTag> using rebind_concept_t = typename rebind_concept<Concept, StructureTag>::type;
}

#include "tag_dispatch/concepts/makeable.hpp"

namespace tag_dispatch::impl
{
template<class ToTag, class StructureTag>
struct to<ToTag, ToTag, StructureTag>
{
	template<class Arg>
	static constexpr decltype( auto ) apply( Arg &&arg )
	{
		return impl::make<ToTag, rebind_concept_t<concepts::makeable, StructureTag>>::apply( std::forward<Arg>( arg ));
	}
};
}

#endif //TAGD_CORE_FUNCTION_DISPATCH_HPP
