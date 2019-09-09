//
// Created by jayz on 30.08.19.
//

#ifndef TAG_DISPATCH_FORWARDING_HPP
#define TAG_DISPATCH_FORWARDING_HPP

#include <type_traits>

#ifdef TAG_DISPATCH_DEFINE_FORWARDING_STATIC_FUNCTION
#error "TAG_DISPATCH_DEFINE_FORWARDING_STATIC_FUNCTION already defined"
#endif

#define TAG_DISPATCH_DEFINE_FORWARDING_STATIC_FUNCTION(name, impl, impl_name) \
template<class ...Args> \
static decltype(auto) name( Args &&...args ) \
{ return impl::impl_name( std::forward<Args>( args )... ); }

#ifdef TAG_DISPATCH_DEFINE_COPYMOVE_STATIC_FUNCTION
#error "TAG_DISPATCH_DEFINE_COPYMOVE_STATIC_FUNCTION already defined"
#endif

#define TAG_DISPATCH_DEFINE_COPYMOVE_STATIC_FUNCTION(function_name, classname, impl, impl_name) \
template<class ...Args> \
static auto function_name( Args &&...args ) \
{ \
    return impl::impl_name( \
        static_cast< \
            std::conditional_t< \
                std::is_rvalue_reference_v<Args &&>, \
                decltype(std::move(args)), \
                classname \
            > \
        >( args )... \
    ); \
}

#endif //TAG_DISPATCH_FORWARDING_HPP
