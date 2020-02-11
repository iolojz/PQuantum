//
// Created by jayz on 17.01.20.
//

#include "parsing.hpp"

#include "parsing_impl.hpp"
#include "instantiations.hpp"

#include "type_rules/variant.hpp"

namespace PQuantum::parsing {
typename instantiations::arithmetic_tree_tag::type parse_arithmetic_expression( const std::string &str ) {
	return phrase_parse_type_skip_space_error<typename instantiations::arithmetic_tree_tag::type>(
		str.cbegin(), str.cend()
	);
}
}
