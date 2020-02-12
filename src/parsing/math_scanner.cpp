//
// Created by jayz on 10.02.20.
//

#include "math_scanner.hpp"

namespace {
template<class CharT>
bool is_valid_atom_character( CharT c ) {
	return std::isalnum( c ) || (c == '\\');
}
}

namespace PQuantum::parsing::bison {
parser::symbol_type yylex( scanner_state &state ) {
	auto &current = state.current;
	const auto &end = state.end;
	
	if( current == end )
		return parser::make_END_OF_INPUT( input_location{ end, end } );
	
	auto first = current++;
	input_location current_location = { first, current };
	
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
	
	current = std::find_if_not( first, end, [] ( const auto &c ) {
		return is_valid_atom_character( c );
	} );
	
	if( current == first ) {
		// Error reporting
	}
	
	current_location.end = current;
	return parser::make_ATOM( atom{ first, current }, std::move( current_location ) );
}
}
