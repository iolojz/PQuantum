//
// Created by jayz on 31.07.19.
//

#ifndef PQUANTUM_MODEL_SPECIFICATION_HPP
#define PQUANTUM_MODEL_SPECIFICATION_HPP

#include <boost/property_tree/ptree_fwd.hpp>
#include <map>

#include "classical_field.hpp"
#include "lagrangian.hpp"

#include "mathutils/manifold_specification.hpp"

namespace PQuantum
{
namespace io
{
struct json_schema_header;
}

namespace model
{
class model_specification
{
	std::string model_name;
	mathutils::manifold_specification spacetime_manifold;
	std::map<classical_field_id, classical_field_specification> field_id_map;
	std::map<mathutils::variable_id, std::string> coefficient_id_map;
	lagrangian lag;
public:
	model_specification(std::string &&n, mathutils::manifold_specification &&m,
						std::map<classical_field_id, classical_field_specification> &&fi,
						std::map<mathutils::variable_id, std::string> &&ci,
						lagrangian &&l );
	
	const std::string &name( void ) const
	{ return model_name; }
	
	const mathutils::manifold_specification &spacetime( void ) const
	{ return spacetime_manifold; }
	
	std::vector<classical_field_id> field_ids( void ) const
	{
		std::vector<classical_field_id> fields;
		for( const auto &id_entry : field_id_map )
			fields.push_back( id_entry.first );
		return fields;
	}
	
	const classical_field_specification &field_specification_for_id( const classical_field_id &id ) const
	{
		return field_id_map.at( id );
	}
	
	const lagrangian &model_lagrangian( void ) const
	{ return lag; }
};
}
}

#endif //PQUANTUM_MODEL_SPECIFICATION_HPP
