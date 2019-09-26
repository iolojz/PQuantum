//
// Created by jayz on 24.09.19.
//

#ifndef TAGD_MODELS_STRING_VIEW_HPP
#define TAGD_MODELS_STRING_VIEW_HPP

#include "tag_dispatch/concepts/total_order.hpp"

#include <string_view>

namespace tag_dispatch::impl
{
template<>
struct make<std::string_view, concepts::makeable<std::string_view, void>>
{
	static constexpr std::string_view apply( const char *str )
	{ return { str }; }
	
	static constexpr std::string_view apply( const std::string_view &str )
	{ return str; }
};

template<>
struct less<std::string_view, concepts::total_order<std::string_view, void>>
{
	static constexpr bool apply( const std::string_view &s1, const std::string_view &s2 )
	{ return ( s1 < s2 ); }
};

template<>
struct equal<std::string_view, concepts::set<std::string_view, void>>
: equal<std::string_view, concepts::total_order<std::string_view, void>>
{
};
}

TAGD_MODELLED_CONCEPT_IMPLEMENTS_FUNCTION( std::string_view, void, set, equal )

#endif //TAGD_MODELS_STRING_VIEW_HPP
