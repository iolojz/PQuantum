//
// Created by jayz on 19.08.19.
//

#include "model_specification.hpp"

namespace PQuantum::model {
model_specification::model_specification(
	std::string &&n, mathutils::manifold_specification &&m,
	std::map<support::uuid, classical_field_specification> &&fi,
	std::map<support::uuid, std::string> &&pi, lagrangian_node &&l
) : model_name{ std::move( n ) }, spacetime_manifold{ std::move( m ) },
field_id_map{ std::move( fi ) }, parameter_id_map{ std::move( pi ) }, lag{ std::move( l ) } {
	// TODO: assert that the lagrangian only contains correct ids
}
}