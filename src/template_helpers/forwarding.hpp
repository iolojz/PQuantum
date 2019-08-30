//
// Created by jayz on 30.08.19.
//

#ifndef PQUANTUM_FORWARDING_HPP
#define PQUANTUM_FORWARDING_HPP

#include <type_traits>

#ifdef PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION
#error "PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION already defined"
#endif

#define PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( name, impl, impl_name ) \
template<class ...Args> \
static decltype(auto) name( Args &&...args ) \
{ return impl::impl_name( std::forward<Args>( args )... ); }

#ifdef PQUANTUM_DEFINE_COPYMOVE_STATIC_FUNCTION
#error "PQUANTUM_DEFINE_COPYMOVE_STATIC_FUNCTION already defined"
#endif

#define PQUANTUM_DEFINE_COPYMOVE_STATIC_FUNCTION( function_name, classname, impl, impl_name ) \
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

#endif //PQUANTUM_FORWARD_HPP
