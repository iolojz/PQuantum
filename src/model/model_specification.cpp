//
// Created by jayz on 19.08.19.
//

#include "model_specification.hpp"

namespace PQuantum
{
namespace model
{
model_specification::model_specification(std::string &&n, mathutils::manifold_specification &&m,
										 std::map<classical_field_id, classical_field_specification> &&fi,
										 std::map<mathutils::variable_id, std::string> &&ci,
										 lagrangian &&l ) : model_name{ std::move( n ) }, spacetime_manifold{ std::move( m ) },
															field_id_map{std::move(fi)},
															coefficient_id_map{std::move(ci)}, lag{std::move(l)}
{
	// TODO: assert that the lagrangian only contains correct ids
}
}
}