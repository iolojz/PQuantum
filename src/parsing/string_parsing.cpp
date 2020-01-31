//
// Created by jayz on 17.01.20.
//

#include "string_parsing.hpp"

#include "support/parsing.hpp"

#include "logging/logging.hpp"

namespace PQuantum::parsing {
mathutils::tree_expression::arithmetic_tree parse_arithmetic_expression( const std::string &str );
	BOOST_LOG_NAMED_SCOPE( "model::parse_lagrangian()" );
	logging::severity_logger logger;
	
	auto result = support::parsing::parse<mathutils::tree_expression>( str.begin(), str.end() );
	if( result.index() == 0 )
		return std::get<0>( result );
	
	auto parsing_result_tuple = std::get<1>( result );
	if( std::get<0>( paring_result_tuple ) ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error )
			<< "End of input string is not an arithmetic expression: \""
			<< std::string( std::get<1>( parsing_result_tuple ), std::get<2>( parsing_result_tuple ) )
			<< "\"";
		error::exit_upon_error();
	}
	
	BOOST_LOG_SEV( logger, logging::severity_level::error )
		<< "Input string is not an arithmetic expression: \"" << str << "\"";
	error::exit_upon_error();
}
}
