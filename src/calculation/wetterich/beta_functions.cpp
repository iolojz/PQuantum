//
// Created by jayz on 02.04.20.
//

#include "../calculation.hpp"
#include "../beta_function.hpp"
#include "../lagrangian_manipulations.hpp"

#include "../../model/model_specification.hpp"

#include "wetterich_equation.hpp"

namespace PQuantum::calculation::wetterich {
void calculate_beta_functions( const model::model_specification &model ) {
	BOOST_LOG_NAMED_SCOPE( "wetterich::calculate_beta_functions()" );
	auto lagrangian = model.lagrangian();
	auto field_ids = model.field_ids();
	std::size_t max_field_degree = maximum_field_degree( lagrangian );
	
	std::set<std::size_t> field_degrees;
	for( std::size_t field_degree = 1; field_degree <= max_field_degree; field_degree++ )
		field_degrees.emplace( field_degree );
	[[maybe_unused]] auto wetterich_trace_terms = trace_terms_of_degrees(
		lagrangian,
		field_ids,
		field_degrees,
		[&model] ( void ) { return model.new_id(); }
	);
}
}