//
// Created by jayz on 10.02.20.
//

#include "math_scanner.hpp"

namespace {
template<class CharT>
bool is_valid_atom_character( CharT c ) {
	return std::isalnum( c ) || ( c == '\\' );
}
}

namespace PQuantum::parsing::bison {
parser::symbol_type yylex( scanner_state &state ) {
	auto &current = state.current;
	const auto &end = state.end;
	
	while( std::isspace( *current ) ) {
		++current;
	}
	
	if( current == end ) {
		return parser::make_END_OF_INPUT( input_location{end, end} );
	}
	
	auto first = current++;
	input_location current_location = {first, current};
	
	switch( *first ) {
	case '+':
		return parser::make_PLUS( std::move( current_location ) );
	case '-':
		return parser::make_MINUS( std::move( current_location ) );
	case '*':
		return parser::make_ASTERISK( std::move( current_location ) );
	case '/':
		return parser::make_SLASH( std::move( current_location ) );
	case '^':
		return parser::make_CARET( std::move( current_location ) );
	case '_':
		// We allow for '_'s in identifiers.
		if( current == end || *current != '{' ) {
			break;
		}
		return parser::make_UNDERSCORE( std::move( current_location ) );
	case ',':
		return parser::make_COMMA( std::move( current_location ) );
	case '(':
		return parser::make_LEFT_ROUND_BRACE( std::move( current_location ) );
	case ')':
		return parser::make_RIGHT_ROUND_BRACE( std::move( current_location ) );
	case '{':
		return parser::make_LEFT_CURLY_BRACE( std::move( current_location ) );
	case '}':
		return parser::make_RIGHT_CURLY_BRACE( std::move( current_location ) );
	}
	
	for( current = first; current != end; ++current ) {
		if( *current == '_' ) {
			auto next = std::next( current );
			if( next != end && *next == '{' ) {
				break;
			}
			continue;
		}
		if( is_valid_atom_character( *current ) == false ) {
			break;
		}
	}
	
	if( current == first ) {
		std::string error_message = "Unrecognized character '";
		error_message.push_back( *first );
		error_message.push_back( '\'' );
		
		throw parser::syntax_error( std::move( current_location ), std::move( error_message ) );
	}
	
	current_location.end = current;
	return parser::make_ATOM( mathutils::string_atom{first, current}, std::move( current_location ) );
}
}
