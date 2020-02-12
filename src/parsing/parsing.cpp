//
// Created by jayz on 11.02.20.
//

#include "parsing.hpp"
#include "parsing/math_scanner.hpp"

#include "error/error.hpp"
#include "logging/logging.hpp"

namespace PQuantum::parsing {
tree_node parse_math( const std::string &str ) {
	BOOST_LOG_NAMED_SCOPE( "parsing::parse_math()" );
	logging::severity_logger logger;

	tree_node result;
	scanner_state state{ str.cbegin(), str.cend() };
	bison::parser parser{ state, result };
	
	if( parser() != 0 ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Cannot parse math string \"" << str << "\".";
		error::exit_upon_error();
	}
	
	return result;
}
}