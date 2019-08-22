//
// Created by jayz on 19.08.19.
//

#include "model_specification.hpp"

namespace PQuantum
{
namespace model
{
model_specification::model_specification( std::string &&n, mathutils::manifold_specification &&m,
										  std::vector<classical_field_specification> &&f ) : model_name(
std::move( n )), spacetime_manifold( std::move( m ))
{
	for( const auto &field_spec : f )
		field_id_map.emplace( classical_field_id{}, std::move( field_spec ));
	std::transform( field_id_map.begin(), field_id_map.end(), std::back_inserter( fields ),
					[]( const auto &key_value ) {
						return key_value.first;
					} );
}
}
}