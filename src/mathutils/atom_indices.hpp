//
// Created by jayz on 12.02.20.
//

#ifndef PQUANTUM_MATHUTILS_INDICES_HPP
#define PQUANTUM_MATHUTILS_INDICES_HPP

#include <vector>

#include <boost/hana.hpp>

namespace PQuantum::mathutils {
template<class IndexAtom>
using index_list = std::vector<IndexAtom>;

template<class IndexAtom>
struct index_spec {
	index_list<IndexAtom> lower, upper;
	
	bool operator==( const index_spec &other ) const {
		return (lower == other.lower && upper == other.upper);
	}
};

template<class Atom, class IndexAtom = Atom>
struct atom_with_optional_indices {
	Atom atom;
	index_spec<IndexAtom> indices;
	
	bool operator==( const atom_with_optional_indices &other ) const {
		return (atom == other.atom && indices == other.indices);
	}
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

template<class Atom>
std::ostream &operator<<( std::ostream &os, const atom_with_optional_indices<Atom> &indexed_atom ) {
	return os << indexed_atom.atom << indexed_atom.indices;
}
}

#endif //PQUANTUM_MATHUTILS_INDICES_HPP
