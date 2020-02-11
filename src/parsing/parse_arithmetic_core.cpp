//
// Created by jayz on 06.02.20.
//

#include "parsing.hpp"

#include "parsing_impl.hpp"
#include "instantiations.hpp"

#include "type_rules/variant.hpp"

instantiations::arithmetic_core_expr parse_arithmetic_core_expression( const std::string &str, qft_parsing_context context ) {
	return phrase_parse_type_skip_space_error<instantiations::arithmetic_core_expr>(
		str.cbegin(), str.cend()
	);
}