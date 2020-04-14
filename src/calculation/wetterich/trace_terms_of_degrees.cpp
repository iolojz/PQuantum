//
// Created by jayz on 14.04.20.
//

#include "wetterich_equation.hpp"
#include "../lagrangian_manipulations.hpp"

namespace PQuantum::calculation::wetterich {
std::map<std::size_t, model::lagrangian_tree> trace_terms_of_degrees(
	const model::lagrangian_tree &lagrangian,
	std::set<support::uuid> field_ids,
	std::set<std::size_t> field_degrees,
	const std::function<support::uuid(void)> &uuid_gen
) {
	if( std::empty( field_degrees ) )
		return {};
	std::size_t maximum_derivative_degree = (* --std::end( field_degrees )) + 2;
	
	std::set<std::size_t> derivative_degrees;
	for( std::size_t derivative_degree = 2; derivative_degree <= maximum_derivative_degree; derivative_degree++ )
		derivative_degrees.emplace( derivative_degree );
	auto expansion = expand_in_field_degrees(
		lagrangian, field_ids, derivative_degrees, uuid_gen
	);
	
	for( const auto &term : expansion )
		std::cout << "degree = " << term.first << ": " << term.second << std::endl;
	
	return {};
}
}
