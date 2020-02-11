//
// Created by jayz on 11.02.20.
//

#include "parsing.hpp"

#include "math_scanner.hpp"

namespace PQuantum::parsing {
tree_node parse_math( const std::string &str ) {
	scanner_state state{ str.cbegin(), str.cend() );
	bison::parser parser{ state };
	
	if( parser() != 0 ) {
		// Something went wrong
	}
	
	return {};
}
}