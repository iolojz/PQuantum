//
// Created by jayz on 17.01.20.
//

#ifndef PQUANTUM_PARSING_STRING_PARSING_HPP
#define PQUANTUM_PARSING_STRING_PARSING_HPP

#include <string>

#include "mathutils/arithmetic.hpp"

namespace PQuantum::parsing {
using math_tree_tag = support::tree::tree_tag<
	mathutils::arithmetic_node_traits,
	arithmetic_core_node_traits
>;

parse_math( const std::string &str );
}

#endif //PQUANTUM_PARSING_STRING_PARSING_HPP
