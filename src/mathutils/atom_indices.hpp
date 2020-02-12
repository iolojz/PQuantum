//
// Created by jayz on 12.02.20.
//

#ifndef PQUANTUM_MATHUTILS_INDICES_HPP
#define PQUANTUM_MATHUTILS_INDICES_HPP

#include "atom.hpp"

#include <vector>

namespace PQuantum::mathutils {
using index_list = std::vector<atom>;

struct index_spec {
	index_list lower, upper;
};

struct atom_with_optional_indices {
	decltype(atom::name) name;
	index_spec indices;
};

struct indexed_atoms_node_traits {
	static constexpr auto node_data_types = boost::hana::tuple_t<atom_with_optional_indices>;
};

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const index_spec &is ) {
	if( is.lower.empty() == false ) {
		os << "_{" << is.lower.front().name;
		for( auto it = ++(is.lower.begin()); it != is.lower.end(); ++it )
			os << ", " << it->name;
		os << "}";
	}
	
	if( is.upper.empty() == false ) {
		os << "^{" << is.lower.front().name;
		for( auto it = ++(is.upper.begin()); it != is.upper.end(); ++it )
			os << ", " << it->name;
		os << "}";
	}
	
	return os;
}
}

PQUANTUM_TREE_DEFINE_NODE_ARITY(mathutils::atom_with_optional_indices, 0)

namespace PQuantum::support::tree {
template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const node_incarnation<mathutils::atom_with_optional_indices, TreeNode> &ni ) {
	return os << ni.data.name << ni.data.indices;
}
}

#endif //PQUANTUM_MATHUTILS_INDICES_HPP
