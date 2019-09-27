//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_CORE_FUNCTION_DISPATCH_HPP
#define CXXMATH_CORE_FUNCTION_DISPATCH_HPP

#include "dispatch_tags.hpp"

namespace cxxmath
{
namespace detail
{
template<class T>
struct always_false
{
	static constexpr bool value = false;
};
template<class T> static constexpr bool always_false_v = always_false<T>::value;
}

template<class Tag>
struct no_default_concept
{
	static_assert( detail::always_false_v<Tag>, "No default concept implemented." );
};

#define CXXMATH_DEFINE_DEFAULT_DISPATCHED_FUNCTION( function, default_concept ) \
struct default_ ## function ## _dispatch { \
    template<class DispatchTag> static constexpr bool supports_tag( void ) { \
        return default_concept<DispatchTag>::function.template supports_tag<DispatchTag>(); \
    }\
    \
    template<class ...Args> static constexpr decltype(auto) apply( Args &&... args ) { \
        using dispatch_tag = common_tag_t<tag_of_t<Args>...>; \
        return default_concept<dispatch_tag>::function( std::forward<Args>( args )... ); \
    } \
}; \
static constexpr auto function = function_object_v<default_ ## function ## _dispatch>;
}

#endif //CXXMATH_CORE_FUNCTION_DISPATCH_HPP