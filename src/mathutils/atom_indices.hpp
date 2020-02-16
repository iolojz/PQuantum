//
// Created by jayz on 12.02.20.
//

#ifndef PQUANTUM_MATHUTILS_INDICES_HPP
#define PQUANTUM_MATHUTILS_INDICES_HPP

#include <vector>

#include <boost/hana.hpp>

#include "support/tree.hpp"

namespace PQuantum::mathutils {
template<class IndexAtom>
using index_list = std::vector<IndexAtom>;

template<class IndexAtom>
struct index_spec {
	index_list<IndexAtom> lower, upper;
};

template<class Atom, class IndexAtom = Atom>
struct atom_with_optional_indices {
	Atom atom;
	index_spec<IndexAtom> indices;
};

template<class Atom, class IndexAtom = Atom>
struct indexed_atoms_node_traits {
	static constexpr auto node_data_types = boost::hana::tuple_t<atom_with_optional_indices<Atom, IndexAtom>>;
};

template<class IndexAtom>
std::ostream &operator<<( std::ostream &os, const index_spec<IndexAtom> &is ) {
	if( is.lower.empty() == false ) {
		os << "_{" << is.lower.front();
		for( auto it = ++( is.lower.begin() ); it != is.lower.end(); ++it ) {
			os << ", " << *it;
		}
		os << "}";
	}
	
	if( is.upper.empty() == false ) {
		os << "^{" << is.lower.front();
		for( auto it = ++( is.upper.begin() ); it != is.upper.end(); ++it ) {
			os << ", " << *it;
		}
		os << "}";
	}
	
	return os;
}
}

namespace PQuantum::support::tree {
template<class Atom, class IndexAtom>
struct arity_for_node_data_impl<mathutils::atom_with_optional_indices<Atom, IndexAtom>> {
	static constexpr auto apply( void ) {
		return 0;
	}
};

template<class Atom>
std::ostream &
operator<<( std::ostream &os, const mathutils::atom_with_optional_indices<Atom> &indexed_atom ) {
	return os << indexed_atom.atom << indexed_atom.indices;
}

template<class Atom, class TreeNode>
std::ostream &
operator<<( std::ostream &os, const node_incarnation<mathutils::atom_with_optional_indices<Atom>, TreeNode> &ni ) {
	return os << ni.data;
}
}

#endif //PQUANTUM_MATHUTILS_INDICES_HPP
