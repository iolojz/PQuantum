//
// Created by jayz on 06.08.19.
//

#ifndef PQUANTUM_MODEL_LAGRANGIAN_HPP
#define PQUANTUM_MODEL_LAGRANGIAN_HPP

#include "classical_field.hpp"
#include "spacetime_index.hpp"
#include "parameter.hpp"

#include "mathutils/string_atom.hpp"
#include "mathutils/arithmetic.hpp"
#include "mathutils/function_call.hpp"
#include "mathutils/number.hpp"

namespace PQuantum::model {
struct gamma_matrix {
	spacetime_index index;
};

struct spacetime_derivative {
	spacetime_index index;
};
struct dirac_operator {};
}

PQUANTUM_TREE_DEFINE_NODE_ARITY( model::gamma_matrix, 0 )
PQUANTUM_TREE_DEFINE_NODE_ARITY( model::spacetime_derivative, 0 )
PQUANTUM_TREE_DEFINE_NODE_ARITY( model::dirac_operator, 0 )

namespace PQuantum::model {
struct lagrangian_atoms_traits {
	static constexpr auto node_data_types = boost::hana::tuple_t<
		indexed_field,
		indexed_parameter,
		gamma_matrix,
		spacetime_derivative,
		dirac_operator,
		mathutils::number
	>;
};

using lagrangian_tree_tag = support::tree::tree_tag<
	mathutils::function_call_node_traits<mathutils::string_atom>,
	mathutils::arithmetic_node_traits,
	lagrangian_atoms_traits
>;
using lagrangian_node = typename lagrangian_tree_tag::node;
}

#endif //PQUANTUM_MODEL_LAGRANGIAN_HPP
