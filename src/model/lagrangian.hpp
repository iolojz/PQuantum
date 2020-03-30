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

static constexpr auto lagrangian_atom_arity_map = boost::hana::make_map(
	boost::hana::make_pair( boost::hana::type_c<indexed_field>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<indexed_parameter>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<gamma_matrix>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<spacetime_derivative>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<dirac_operator>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<mathutils::number>, boost::hana::int_c<0> )
);
static constexpr auto lagrangian_arity_map = boost::hana::union_(
	boost::hana::union_(
		mathutils::function_call_arity_map<mathutils::string_atom>,
		mathutils::arithmetic_arity_map
	),
	lagrangian_atom_arity_map
);
using lagrangian_tree = cxxmath::typesafe_tree<decltype(lagrangian_arity_map)>;
}

#endif //PQUANTUM_MODEL_LAGRANGIAN_HPP
