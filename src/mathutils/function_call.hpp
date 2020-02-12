//
// Created by jayz on 12.02.20.
//

#ifndef PQUANTUM_MATHUTILS_FUNCTION_CALL_HPP
#define PQUANTUM_MATHUTILS_FUNCTION_CALL_HPP

#include "support/tree.hpp"

#include <string>

namespace PQuantum::mathutils {
struct function_call {
	std::string name;
};

struct function_call_node_traits {
	static constexpr auto node_data_types = boost::hana::tuple_t<function_call>;
};
}

PQUANTUM_TREE_DEFINE_NODE_ARITY(mathutils::function_call, runtime_arity)

namespace PQuantum::support::tree {
template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const node_incarnation<mathutils::function_call, TreeNode> &ni ) {
	return os << ni.data.name << "{";
	if( ni.children.empty() == false ) {
		os << ni.children.front();
		for( auto it = ++(ni.children.begin()); it != ni.children.end(); ++it )
			os << ", " << *it;
	}
	return os << "}";
}
}

#endif //PQUANTUM_MATHUTILS_FUNCTION_CALL_HPP
