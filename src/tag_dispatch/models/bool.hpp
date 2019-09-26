//
// Created by jayz on 09.09.19.
//

#ifndef TAGD_MODELS_BOOL_HPP
#define TAGD_MODELS_BOOL_HPP

#include "tag_dispatch/concepts/boolean_lattice.hpp"

namespace tag_dispatch {
struct std_bool_constant_tag {
};
namespace impl {
// Concrete model: default bool
template<>
struct not_<bool, concepts::boolean_lattice<bool, void>> {
	static bool apply(bool value) { return !value; }
};

template<>
struct and_<bool, concepts::boolean_lattice<bool, void>> {
	template<class ...Values>
	static bool apply(Values... values) { return (values && ...); }
};

template<>
struct or_<bool, concepts::boolean_lattice<bool, void>> {
	template<class ...Values>
	static bool apply(Values... values) { return (values || ...); }
};

template<>
struct equal<bool, concepts::set < bool, void>> {
static bool apply( bool v1, bool v2 )
{ return ( v1 == v2 ); }
};

template<class DispatchTag, class StructureTag>
struct if_;

template<>
struct if_<bool, concepts::boolean_lattice<bool, void>> {
	template<class Then, class Else>
	static auto apply(bool value, Then &&then, Else &&else_) { return value ? then : else_; }
};

// Concrete model: default std::bool_constant
template<>
struct tag_of<std::true_type> {
	using type = std_bool_constant_tag;
};
template<>
struct tag_of<std::false_type> {
	using type = std_bool_constant_tag;
};

template<>
struct not_<std_bool_constant_tag, concepts::boolean_lattice<std_bool_constant_tag, void>> {
	static constexpr std::true_type apply(std::false_type) { return {}; }
	
	static constexpr std::false_type apply(std::true_type) { return {}; }
};

template<>
struct and_<std_bool_constant_tag, concepts::boolean_lattice<std_bool_constant_tag, void>> {
	template<class ...Values>
	static constexpr std::bool_constant<std::conjunction_v<Values...>> apply(Values...) { return {}; }
};

template<>
struct or_<std_bool_constant_tag, concepts::boolean_lattice<std_bool_constant_tag, void>> {
	template<class ...Values>
	static constexpr std::bool_constant<std::disjunction_v<Values...>> apply(Values...) { return {}; }
};

template<>
struct equal<std_bool_constant_tag, concepts::set < std_bool_constant_tag, void>> {
	static constexpr std::true_type apply(std::true_type, std::true_type) { return {}; }
	
	static constexpr std::false_type apply(std::true_type, std::false_type) { return {}; }
	
	static constexpr std::false_type apply(std::false_type, std::true_type) { return {}; }
	
	static constexpr std::true_type apply(std::false_type, std::false_type) { return {}; }
};

template<>
struct if_<std_bool_constant_tag, concepts::boolean_lattice<std_bool_constant_tag, void>> {
	template<class Then, class Else>
	static constexpr auto apply(std::true_type, Then &&then, Else &&) { return then; }
	
	template<class Then, class Else>
	static constexpr auto apply(std::false_type, Then &&, Else &&else_) { return else_; }
};
}

template<class StructureTag, class If, class Then, class Else>
struct function_traits_for_dispatch<concepts::boolean_lattice < bool, StructureTag>, impl::if_, If, Then, Else> {
using dispatch_tag = bool;
using structure_tag = concepts::boolean_lattice<bool, StructureTag>;
using pre_action = impl::identity;
using post_action = impl::identity;
};

template<class StructureTag, class If, class Then, class Else>
struct function_traits_for_dispatch<
concepts::boolean_lattice < std_bool_constant_tag, StructureTag>, impl::if_, If, Then, Else> {
using dispatch_tag = std_bool_constant_tag;
using structure_tag = concepts::boolean_lattice<std_bool_constant_tag, StructureTag>;
using pre_action = impl::identity;
using post_action = impl::identity;
};
}

TAGD_MODELLED_CONCEPT_IMPLEMENTS_FUNCTION( bool, void, boolean_lattice, if_ )
TAGD_MODELLED_CONCEPT_IMPLEMENTS_FUNCTION( bool, void, set, equal )

TAGD_MODELLED_CONCEPT_IMPLEMENTS_FUNCTION( std_bool_constant_tag, void, boolean_lattice, if_ )
TAGD_MODELLED_CONCEPT_IMPLEMENTS_FUNCTION( std_bool_constant_tag, void, set, equal )

TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( if_ )

#endif //TAGD_MODELS_BOOL_HPP
