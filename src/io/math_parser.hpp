//
// Created by jayz on 16.08.19.
//

#ifndef PQUANTUM_MATH_PARSER_HPP
#define PQUANTUM_MATH_PARSER_HPP

#include "logging.hpp"
#include "string_to_uuid.hpp"

#include "error/control.hpp"
#include "mathutils/expressions.hpp"
#include "model/classical_field.hpp"

#include <variant>
#include <mathutils/linear_operators.hpp>
#include <boost/spirit/home/x3.hpp>

namespace PQuantum
{
namespace io
{
namespace parser_rules
{
template<class Symbol> class rule_for_symbol;
}

template<class Symbol>
std::vector<Symbol> parse_symbols( const std::string &string, const string_to_uuid &uuid_gen )
{
	BOOST_LOG_NAMED_SCOPE( "io::parse_symbols()" );
	io::severity_logger logger;
	
	std::vector<Symbol> symbols;
	const auto symbol_parser = parser_rules::rule_for_symbol<Symbol>{ uuid_gen };
	auto it = string.begin();
	
	bool parsing_result = boost::spirit::x3::phrase_parse( it, string.end(), *symbol_parser,
														   boost::spirit::x3::ascii::space /*,
 * 		symbols
 */
	);
	
	if( parsing_result == false ) {
		BOOST_LOG_SEV( logger, severity_level::error ) << "Cannot parse input string: \"" << string << "\"";
		control::exit_upon_error();
	}
	
	if( it != string.end()) {
		BOOST_LOG_SEV( logger, severity_level::error ) << "Cannot parse end of input string: \""
													   << std::string( it, string.end()) << "\"";
	}
	
	return symbols;
}

mathutils::polynomial_expression parse_polynomial_expression( const std::string & );
}
}

#endif //PQUANTUM_MATH_PARSER_HPP
