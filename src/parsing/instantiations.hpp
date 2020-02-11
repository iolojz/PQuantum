//
// Created by jayz on 03.02.20.
//

#include "type_rules/arithmetic.hpp"

#ifndef PQUANTUM_PARSING_INSTANTIATIONS_HPP
#define PQUANTUM_PARSING_INSTANTIATIONS_HPP

namespace PQuantum::parsing::instantiations {
using arithmetic_tree_tag = support::tree::tree_tag<
	mathutils::arithmetic_node_traits,
	arithmetic_core_node_traits
>;

using arithmetic_core_expr = std::vector<
    std::variant<
		dirac_operator, classical_field_id, gamma_matrix, spacetime_derivative, variable_id
    >
>;
}

#endif //PQUANTUM_PARSING_INSTANTIATIONS_HPP
