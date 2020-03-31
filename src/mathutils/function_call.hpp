//
// Created by jayz on 12.02.20.
//

#ifndef PQUANTUM_MATHUTILS_FUNCTION_CALL_HPP
#define PQUANTUM_MATHUTILS_FUNCTION_CALL_HPP

#include <ostream>

namespace PQuantum::mathutils {
template<class Atom>
struct function_call {
	Atom atom;
	
	bool operator==( const function_call &other ) const {
    	return atom == other.atom;
	}
};

template<class Atom>
static constexpr auto function_call_arity_map = boost::hana::make_map(
	boost::hana::make_pair( boost::hana::type_c<function_call<Atom>>, cxxmath::runtime_arity )
);

template<class Atom>
std::ostream &operator<<( std::ostream &os, const function_call<Atom> &fc ) {
	return os << fc.atom;
}
}

#endif //PQUANTUM_MATHUTILS_FUNCTION_CALL_HPP
