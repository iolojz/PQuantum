//
// Created by jayz on 19.08.19.
//

#include "model_specification.hpp"

namespace PQuantum
{
namespace model
{
model_specification::model_specification( std::string &&n, mathutils::manifold_specification &&m,
										  std::map<classical_field_id, classical_field_specification> &&fi,
										  std::map<mathutils::variable_id, std::string> &&pi,
										  lagrangian_tree &&l ) : model_name{ std::move( n ) },
																  spacetime_manifold{ std::move( m ) },
																  field_id_map{ std::move( fi ) },
																  parameter_id_map{ std::move( pi ) },
																  lag{ std::move( l ) }
{
	// TODO: assert that the lagrangian only contains correct ids
}
}
}