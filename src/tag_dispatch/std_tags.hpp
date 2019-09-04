//
// Created by jayz on 03.09.19.
//

#ifndef PQUANTUM_STD_TAGS_HPP
#define PQUANTUM_STD_TAGS_HPP

#include "make.hpp"

#include <tuple>
#include <vector>
#include <utility>

#ifdef PQUANTUM_TAG_STD_TEMPLATE
#error "PQUANTUM_TAG_STD_TEMPLATE already defined"
#endif

#define PQUANTUM_TAG_STD_TEMPLATE(temp) \
template<class ...TemplateArgs> struct tag_of<std::temp<TemplateArgs...>> \
{ \
    using type = std_ ## temp ## _tag; \
};

namespace PQuantum::tag_dispatch
{
struct std_tuple_tag
{
};
struct std_vector_tag
{
};
struct std_pair_tag
{
};

namespace impl
{
PQUANTUM_TAG_STD_TEMPLATE( tuple )
PQUANTUM_TAG_STD_TEMPLATE( vector )
PQUANTUM_TAG_STD_TEMPLATE( pair )

template<>
struct make<std_tuple_tag>
{
	template<class FunctionTag = void, class ...Args>
	static decltype( auto ) apply( Args &&...args )
	{
		return std::make_tuple( std::forward<Args>( args )... );
	}
};

template<>
struct make<std_vector_tag>
{
	template<class AllocatorOrVoid = void, class ...Args>
	static decltype( auto ) apply( Args &&...args )
	{
		using value_type = std::common_type_t<Args...>;
		using allocator_type = std::conditional_t<std::is_void_v<AllocatorOrVoid>, typename std::vector<value_type>::allocator_type, AllocatorOrVoid>;
		
		return std::vector<value_type, allocator_type>{ std::forward<Args>( args )... };
	}
};

template<>
struct make<std_pair_tag>
{
	template<class FunctionTag = void, class First, class Second>
	static decltype( auto ) apply( First &&first, Second &&second )
	{
		return std::make_pair( std::forward<First>( first ), std::forward<Second>( second ));
	}
};

namespace functions
{
template<class Tag>
struct make
{
	template<class FunctionTag = void, class ...Args>
	decltype( auto ) apply( Args &&...args ) const
	{ return impl::make<Tag>::template apply<FunctionTag>( std::forward<Args>( args )... ); }
	
	template<class ...Args>
	decltype( auto ) operator()( Args &&...args ) const
	{ return apply( std::forward<Args>( args )... ); }
};
}
}

static constexpr functions::make <std_tuple_tag> make_tuple;
static constexpr functions::make <std_vector_tag> make_vector;
static constexpr functions::make <std_pair_tag> make_pair;
}

#undef PQUANTUM_TAG_STD_TEMPLATE

#endif //PQUANTUM_STD_TAGS_HPP
