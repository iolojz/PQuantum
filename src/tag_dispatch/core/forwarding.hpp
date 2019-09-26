//
// Created by jayz on 30.08.19.
//

#ifndef TAGD_CORE_FORWARDING_HPP
#define TAGD_CORE_FORWARDING_HPP

#ifdef TAGD_DEFINE_FORWARDING_FUNCTION
#error "TAGD_DEFINE_FORWARDING_FUNCTION already defined"
#endif
#define TAGD_DEFINE_FORWARDING_FUNCTION(name, implementation, rebind_structure_tag) \
template<class DispatchTag, class StructureTag> struct name { \
private: \
    using underlying_implementation = implementation<DispatchTag, rebind_structure_tag<DispatchTag, StructureTag>>; \
public: \
    template<class ...Args> \
    static constexpr decltype(auto) apply( Args &&...args ) \
    { return underlying_implementation::apply( std::forward<Args>( args )... ); } \
};

#ifdef TAGD_DEFINE_CONCEPT_FORWARDING_FUNCTION
#error "TAGD_DEFINE_CONCEPT_FORWARDING_FUNCTION already defined"
#endif
#define TAGD_DEFINE_CONCEPT_FORWARDING_FUNCTION(function_name, concept_name, implementation, rebind_structure_tag) \
template<class DispatchTag, class StructureTag> struct function_name<DispatchTag, concepts::concept_name<DispatchTag, StructureTag>> \
{ \
private: \
    using underlying_implementation = implementation<DispatchTag, rebind_structure_tag<DispatchTag, concepts::concept_name<DispatchTag, StructureTag>>>; \
public: \
    template<class ...Args> \
    static constexpr decltype(auto) apply( Args &&...args ) \
    { return underlying_implementation::apply( std::forward<Args>( args )... ); } \
};

#endif //TAGD_CORE_FORWARDING_HPP
