//
// Created by jayz on 02.04.20.
//

#include "calculation.hpp"

#include "lagrangian_manipulations.hpp"

namespace PQuantum::calculation {
void calculate_beta_functions_with_wetterich_equation( const model::model_specification &model ) {
	BOOST_LOG_NAMED_SCOPE( "calculate_beta_functions_with_wetterich_equation()" );
	auto lagrangian = model.lagrangian();
	auto field_ids = model.field_ids();
	std::map<support::uuid, support::uuid> delta1_field_map, delta2_field_map;
	
	for( const auto &field_id : field_ids ) {
		delta1_field_map[field_id] = model.new_id();
		delta2_field_map[field_id] = model.new_id();
	}
	
	auto second_derivative = to_polynomial( simplify_arithmetic(
		take_derivative( delta1_field_map, take_derivative( delta2_field_map, lagrangian ) )
	) );
	
	std::cout << second_derivative << std::endl;
}
}