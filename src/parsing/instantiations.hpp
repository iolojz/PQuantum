//
// Created by jayz on 03.02.20.
//

#include "type_rules/arithmetic.hpp"

#ifndef PQUANTUM_PARSING_INSTANTIATIONS_HPP
#define PQUANTUM_PARSING_INSTANTIATIONS_HPP

namespace PQuantum::parsing::instantiations {
using arithmetic_tree_tag = support::tree::tree_tag<
		mathutils::tree_expression::arithmetic_node_traits,
		identifier_node_traits
>;
}

#endif //PQUANTUM_PARSING_INSTANTIATIONS_HPP
