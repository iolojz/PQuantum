//
// Created by jayz on 02.04.20.
//

#include "calculation.hpp"
#include "lagrangian_manipulations.hpp"

#include "../model/model_specification.hpp"

namespace PQuantum::calculation {
void calculate_beta_functions_with_wetterich_equation( const model::model_specification &model ) {
	BOOST_LOG_NAMED_SCOPE( "calculate_beta_functions_with_wetterich_equation()" );
	auto lagrangian = model.lagrangian();
	auto field_ids = model.field_ids();
	std::map<support::uuid, support::uuid> delta1_field_map, delta2_field_map;
	
	for( const auto &field_id : field_ids ) {
		delta1_field_map.emplace( field_id, model.new_id() );
		delta2_field_map.emplace( field_id,  model.new_id() );
	}
	
	std::size_t max_field_degree = maximum_field_degree( lagrangian );
	
	auto second_derivative = simplify_arithmetic(
		take_derivative( delta1_field_map, take_derivative( delta2_field_map, lagrangian ) )
	);
	auto second_derivative_at_zero = zero_fields( second_derivative, field_ids );
	
	std::vector<std::map<support::uuid, support::uuid>> higher_delta_maps;
	std::vector<model::lagrangian_tree> full_higher_derivatives;
	if( max_field_degree != 2 ) {
		higher_delta_maps.emplace_back();
		for( const auto &field_id : field_ids ) {
			higher_delta_maps.front().emplace( field_id, model.new_id() );
		}
		full_higher_derivatives.push_back( simplify_arithmetic(
			take_derivative( higher_delta_maps.front(), second_derivative )
		) );
	}
	
	for( std::size_t degree = 2; degree != max_field_degree; ++degree ) {
		higher_delta_maps.emplace_back();
		for( const auto &field_id : field_ids ) {
			higher_delta_maps.back().emplace( field_id, model.new_id() );
		}
		full_higher_derivatives.push_back( simplify_arithmetic(
			take_derivative( higher_delta_maps.back(), full_higher_derivatives.back() )
		) );
	}
	
	std::vector<model::lagrangian_tree> higher_derivative_operators_at_zero;
	std::vector<support::uuid> higher_delta_fields;
	for( auto delta_map_it = ++(++std::begin( higher_delta_maps ));
		delta_map_it != std::end( higher_delta_maps );
		++delta_map_it ) {
		for( const auto &map_entry : *delta_map_it )
			higher_delta_fields.push_back( map_entry.second );
	}
	
	std::transform(
		std::make_move_iterator( std::begin( full_higher_derivatives ) ),
		std::make_move_iterator( std::end( full_higher_derivatives ) ),
		std::back_inserter( higher_derivative_operators_at_zero ),
		[] ( auto &&full_derivative ) {
			return zero_fields( std::move( full_derivative ), higher_delta_fields );
		}
	);
	
	std::vector<model::lagrangian_tree> wetterich_rhs_trace_terms;
	for( std::size_t degree = 1; degree != max_field_degree; ++degree ) {
		model::lagrangian_tree term = {
			mathutils::quotient{},
			mathutils::number{ 1, 0 },
			mathutils::number{ 2, 0 },
		};
		wetterich_rhs_trace_terms.push_back( std::move( term ) );
	}
	
	std::cout << "degree = " << max_field_degree << std::endl;
	std::cout << second_derivative_at_zero << std::endl;
}
}