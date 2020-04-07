//
// Created by jayz on 06.08.19.
//

#ifndef PQUANTUM_MODEL_LAGRANGIAN_HPP
#define PQUANTUM_MODEL_LAGRANGIAN_HPP

#include "classical_field.hpp"
#include "parameter.hpp"

#include "mathutils/arithmetic.hpp"
#include "mathutils/function_call.hpp"
#include "mathutils/number.hpp"
#include "mathutils/linear_operators.hpp"

namespace PQuantum::model {
static constexpr auto input_lagrangian_atom_arity_map = boost::hana::make_map(
	boost::hana::make_pair( boost::hana::type_c<indexed_field>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<indexed_parameter>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<mathutils::gamma_matrix>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<mathutils::spacetime_derivative>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<mathutils::dirac_operator>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<mathutils::number>, boost::hana::int_c<0> )
);
static constexpr auto input_lagrangian_arity_map = boost::hana::union_(
	mathutils::arithmetic_arity_map,
	input_lagrangian_atom_arity_map
);
using input_lagrangian_tree = cxxmath::typesafe_tree<decltype(input_lagrangian_arity_map)>;

static constexpr auto lagrangian_atom_arity_map = boost::hana::make_map(
	boost::hana::make_pair( boost::hana::type_c<indexed_field>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<indexed_parameter>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<mathutils::gamma_matrix>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<mathutils::number>, boost::hana::int_c<0> )
);
static constexpr auto lagrangian_operator_arity_map = boost::hana::make_map(
	boost::hana::make_pair( boost::hana::type_c<mathutils::spacetime_derivative>, boost::hana::int_c<1> ),
	boost::hana::make_pair( boost::hana::type_c<mathutils::dirac_operator>, boost::hana::int_c<1> )
);
static constexpr auto lagrangian_arity_map = boost::hana::union_(
	boost::hana::union_(
		boost::hana::union_(
			mathutils::arithmetic_arity_map,
			mathutils::function_call_arity_map<std::string>
		),
		lagrangian_operator_arity_map
	),
	lagrangian_atom_arity_map
);
using lagrangian_tree = cxxmath::typesafe_tree<decltype(lagrangian_arity_map)>;
}

#endif //PQUANTUM_MODEL_LAGRANGIAN_HPP
