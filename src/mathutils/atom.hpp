//
// Created by jayz on 12.02.20.
//

#ifndef PQUANTUM_MATHUTILS_ATOM_HPP
#define PQUANTUM_MATHUTILS_ATOM_HPP

namespace PQuantum::mathutils {
struct atom {
	std::string name;
	
	atom( void ) = default;
	atom( const atom & ) = default;
	atom( atom && ) = default;
	
	atom &operator=( const atom & ) = default;
	atom &operator=( atom && ) = default;
	
	template<class Arg1, class ...Args, class = std::enable_if_t<!std::is_same_v<std::decay_t<Arg1>, atom>>>
	atom( Arg1 &&arg1, Args &&...args )
			: name{ std::forward<Arg1>( arg1 ), std::forward<Args>( args )... } {}
};
}

#endif //PQUANTUM_MATHUTILS_ATOM_HPP
