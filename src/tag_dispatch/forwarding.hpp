//
// Created by jayz on 30.08.19.
//

#ifndef TAGD_FORWARDING_HPP
#define TAGD_FORWARDING_HPP

#ifdef TAGD_DEFINE_FORWARDING_FUNCTION
#error "TAGD_DEFINE_FORWARDING_FUNCTION already defined"
#endif
#define TAGD_DEFINE_FORWARDING_FUNCTION(name, implementation, rebind_structure_tag) \
template<class DispatchTag, class StructureTag> struct name { \
private: \
    using underlying_action = implementation<DispatchTag, rebind_structure_tag<DispatchTag, StructureTag>>; \
public: \
    template<class ...Args> \
    static decltype(auto) apply( Args &&...args ) \
    { return underlying_action::apply( std::forward<Args>( args )... ); } \
};

#endif //TAGD_FORWARDING_HPP
