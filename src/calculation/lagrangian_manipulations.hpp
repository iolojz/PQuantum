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

model::lagrangian_tree simplify_arithmetic( const model::lagrangian_tree &expr );

model::lagrangian_tree field_derivative(
	const model::lagrangian_tree &expr,
	const std::map<support::uuid, support::uuid> &delta_field_map
);

std::size_t maximum_field_degree( const model::lagrangian_tree &expr );

std::map<std::size_t, model::lagrangian_tree> expand_in_field_degrees(
	const model::lagrangian_tree &expr,
	const std::set<support::uuid> &field_ids,
	const std::set<std::size_t> &field_degrees,
	const std::function<support::uuid(void)> &uuid_gen
);

model::lagrangian_tree zero_fields( const model::lagrangian_tree &expr, const std::set<support::uuid> &field_ids );
model::lagrangian_tree map_fields(
	const model::lagrangian_tree &expr,
	const std::map<support::uuid, support::uuid> &field_id_map
);

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const euler_constant & ) {
	return os << "\\EulerConstant";
}

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const pi_constant & ) {
	return os << "\\pi";
}
}

#endif //PQUANTUM_CALCULATION_LAGRANGIAN_MANIPULATIONS_HPP
