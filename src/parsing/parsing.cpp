//
// Created by jayz on 17.01.20.
//

#include "parsing.hpp"

#include "parsing_impl.hpp"
#include "lazy_instantiations.hpp"

#include "type_rules/variant.hpp"

namespace PQuantum::parsing {
PQUANTUM_PARSING_ARITHMETIC_TREE_NODE parse_arithmetic_expression( const std::string &str ) {
	return phrase_parse_type_skip_space_error<PQUANTUM_PARSING_ARITHMETIC_TREE_NODE>(
		str.cbegin(), str.cend()
	);
}
}
