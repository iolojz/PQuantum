//
// Created by jayz on 14.04.20.
//

#include "lagrangian_manipulations.hpp"

#include <future>

namespace PQuantum::calculation {
std::map<std::size_t, model::lagrangian_tree> expand_in_field_degrees(
	const model::lagrangian_tree &expr,
	const std::set<support::uuid> &field_ids,
	const std::set<std::size_t> &field_orders,
	const std::function<support::uuid(void)> &uuid_gen
) {
	if( field_orders.size() == 0 )
		return {};
	
	std::size_t max_field_degree = *(--std::end( field_orders ));
	if( max_field_degree == 0 )
		return { std::make_pair( 0, simplify_arithmetic( zero_fields( expr, field_ids ) ) ) };
	
	std::vector<std::map<support::uuid, support::uuid>> delta_field_id_maps;
	std::vector<model::lagrangian_tree> derivatives = { expr };
	
	auto initial_delta_field_id_map = [&field_ids, &uuid_gen] ( void ) {
		std::map<support::uuid, support::uuid> map;
		for( const auto &field_id : field_ids )
			map.emplace( field_id, uuid_gen() );
		return map;
	};
	auto iterate_delta_field_id_map = [&uuid_gen] ( auto &map ) {
		for( auto &field_delta_pair : map ) {
			field_delta_pair.second = uuid_gen();
		}
	};
	
	delta_field_id_maps.reserve( max_field_degree );
	derivatives.reserve( max_field_degree + 1 );
	
	auto current_delta_field_id_map = initial_delta_field_id_map();
	do {
		derivatives.push_back( simplify_arithmetic(
			field_derivative( derivatives.back(), current_delta_field_id_map )
		) );
		delta_field_id_maps.push_back( current_delta_field_id_map );
		
		if( delta_field_id_maps.size() == max_field_degree )
			break;
		iterate_delta_field_id_map( current_delta_field_id_map );
	} while( true );
	
	std::map<support::uuid, support::uuid> reverse_delta_map;
	for( const auto &delta_field_id_map : delta_field_id_maps ) {
		for( const auto &entry : delta_field_id_map )
			reverse_delta_map.emplace( entry.second, entry.first );
	}
	
	std::map<std::size_t, model::lagrangian_tree> expansion;
	std::vector<std::pair<std::size_t, std::future<model::lagrangian_tree>>> lazy_derivatives;
	
	auto field_order_it = std::begin( field_orders );
	if( *field_order_it == 0 ) {
		expansion.emplace( 0, zero_fields( derivatives.front(), field_ids ) );
		++field_order_it;
	}
	
	while( field_order_it != std::end( field_orders ) ) {
		lazy_derivatives.emplace_back(
			*field_order_it,
			std::async(
				[&field_ids, &reverse_delta_map] ( const auto &derivative ) {
					return map_fields( zero_fields( derivative, field_ids ), reverse_delta_map );
				},
				derivatives[*field_order_it]
			)
		);
		++field_order_it;
	}
	
	for( auto &&lazy_derivative : lazy_derivatives ) {
		std::size_t degree = lazy_derivative.first;
		auto derivative = lazy_derivative.second.get();
		
		model::lagrangian_tree expansion_term = {
			mathutils::quotient{},
			std::move( derivative ),
			model::lagrangian_tree{
				mathutils::function_call<std::string>{ "factorial" },
				mathutils::number{ degree, 0 }
			}
		};
		
		expansion.emplace(
			std::move( degree ),
			simplify_arithmetic( std::move( expansion_term ) )
		);
	}
	
	return expansion;
}
}
