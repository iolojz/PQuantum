//
// Created by jayz on 06.08.19.
//

#ifndef PQUANTUM_MODEL_LAGRANGIAN_HPP
#define PQUANTUM_MODEL_LAGRANGIAN_HPP

#include "support/tree.hpp"

#include "mathutils/number.hpp"
#include "mathutils/variable.hpp"
#include "mathutils/linear_operators.hpp"

#include "classical_field.hpp"

namespace PQuantum::model {
static constexpr auto non_arithmetic_input_types = boost::hana::tuple_t<
	mathutils::linear_operators::gamma_matrix, mathutils::linear_operators::spacetime_derivative,
	mathutils::linear_operators::dirac_operator,
	mathutils::variable_id, mathutils::number, classical_field_id
>;

using non_arithmetic_input_tree = support::tree::tree_node<non_arithmetic_input_types>;
}

#endif //PQUANTUM_MODEL_LAGRANGIAN_HPP
