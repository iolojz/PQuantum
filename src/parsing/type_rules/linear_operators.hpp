//
// Created by jayz on 05.02.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULES_LINEAR_OPERATORS_HPP
#define PQUANTUM_PARSING_TYPE_RULES_LINEAR_OPERATORS_HPP

#include "mathutils/linear_operators.hpp"

#include "../x3_helpers.hpp"

namespace PQuantum::parsing {
template<class TreeTag>
struct type_rule_impl<mathutils::linear_operators::gamma_matrix> {
	static auto apply(void) {
		return "\\gamma" >> make_type_rule<spacetime_index>;
	}
};

struct type_rule_impl<mathutils::linear_operators::spacetime_derivative> {
	static auto apply(void) {
		return "\\partial" >> make_type_rule<spacetime_index>;
	}
};

struct type_rule_impl<mathutils::linear_operators::dirac_operator>> {
	static auto apply(void) {
		return "\\DiracOperator";
	}
};
}

#endif //PQUANTUM_PARSING_TYPE_RULES_LINEAR_OPERATORS_HPP
