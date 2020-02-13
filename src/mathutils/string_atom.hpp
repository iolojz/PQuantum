//
// Created by jayz on 12.02.20.
//

#ifndef PQUANTUM_MATHUTILS_STRING_ATOM_HPP
#define PQUANTUM_MATHUTILS_STRING_ATOM_HPP

namespace PQuantum::mathutils {
struct string_atom {
	std::string name;
	
	string_atom(void ) = default;
	string_atom(const string_atom & ) = default;
	string_atom(string_atom && ) = default;
	
	string_atom &operator=(const string_atom & ) = default;
	string_atom &operator=(string_atom && ) = default;
	
	template<class Arg1, class ...Args, class = std::enable_if_t<!std::is_same_v<std::decay_t<Arg1>, string_atom>>>
	string_atom(Arg1 &&arg1, Args &&...args )
	: name{ std::forward<Arg1>( arg1 ), std::forward<Args>( args )... } {}
};
}

#endif //PQUANTUM_MATHUTILS_STRING_ATOM_HPP
