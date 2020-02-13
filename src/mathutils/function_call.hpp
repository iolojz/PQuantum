//
// Created by jayz on 12.02.20.
//

#ifndef PQUANTUM_MATHUTILS_FUNCTION_CALL_HPP
#define PQUANTUM_MATHUTILS_FUNCTION_CALL_HPP

#include "support/tree.hpp"

#include <string>

namespace PQuantum::mathutils {
template<class Atom>
struct function_call {
	Atom atom;
};

template<class Atom>
struct function_call_node_traits {
	static constexpr auto node_data_types = boost::hana::tuple_t<function_call<Atom>>;
};
}

namespace PQuantum::support::tree {
template<class Atom> struct arity_for_node_data_impl<mathutils::function_call<Atom>> {
	static constexpr auto apply( void ) {
		return runtime_arity;
	}
};

template<class Atom, class TreeNode>
std::ostream &operator<<( std::ostream &os, const node_incarnation<mathutils::function_call<Atom>, TreeNode> &ni ) {
	return os << ni.atom << "{";
	if( ni.children.empty() == false ) {
		os << ni.children.front();
		for( auto it = ++(ni.children.begin()); it != ni.children.end(); ++it )
			os << ", " << *it;
	}
	return os << "}";
}
}

#endif //PQUANTUM_MATHUTILS_FUNCTION_CALL_HPP
