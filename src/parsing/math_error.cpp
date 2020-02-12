//
// Created by jayz on 12.02.20.
//

#include "logging/logging.hpp"

#include "math_types.hpp"
#include "math_scanner.hpp"
#include "math_parse.hpp"

namespace PQuantum::parsing::bison {
void parser::error( const input_location &loc, const std::string &error_message ) {
	BOOST_LOG_NAMED_SCOPE("parsing::bison::yyerror()");
	logging::severity_logger logger;
	BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Cannot parse string \"" <<
		std::string{ loc.begin, loc.end } << "\": " << error_message;
}
}