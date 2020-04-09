//
// Created by jayz on 07.04.20.
//

#ifndef PQUANTUM_CALCULATION_LAGRANGIAN_MANIPULATIONS_HPP
#define PQUANTUM_CALCULATION_LAGRANGIAN_MANIPULATIONS_HPP

#include "../model/lagrangian.hpp"

namespace PQuantum::calculation {
struct euler_constant {
	constexpr bool operator==( const euler_constant & ) const { return true; }
};

struct pi_constant {
	constexpr bool operator==( const pi_constant & ) const { return true; }
};

std::size_t maximum_field_degree( const model::lagrangian_tree &expr );

model::lagrangian_tree simplify_arithmetic( const model::lagrangian_tree &expr );

model::lagrangian_tree take_derivative(
	const std::map<support::uuid, support::uuid> &delta_field_map,
	const model::lagrangian_tree &expr
);

model::lagrangian_tree zero_fields( const model::lagrangian_tree &expr, const std::vector<support::uuid> &field_ids );

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const euler_constant & ) {
	return os << "\\EulerConstant";
}

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const pi_constant & ) {
	return os << "\\pi";
}
}

#endif //PQUANTUM_CALCULATION_LAGRANGIAN_MANIPULATIONS_HPP
