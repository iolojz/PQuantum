//
// Created by jayz on 31.07.19.
//

#ifndef PQUANTUM_MODEL_SPECIFICATION_HPP
#define PQUANTUM_MODEL_SPECIFICATION_HPP

#include <boost/property_tree/ptree_fwd.hpp>
#include <map>

#include "error/control.hpp"
#include "io/logging.hpp"

#include "classical_field.hpp"
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
	std::vector<classical_field_id> fields;
	std::map<classical_field_id, classical_field_specification> field_id_map;
public:
	model_specification( std::string &&n, mathutils::manifold_specification &&m,
						 std::vector<classical_field_specification> &&f );
	
	const std::string &name( void ) const
	{ return model_name; }
	
	const mathutils::manifold_specification &spacetime( void ) const
	{ return spacetime_manifold; }
	
	const std::vector<classical_field_id> &field_ids( void ) const
	{ return fields; }
	
	const classical_field_specification &field_specification_for_id( const classical_field_id &id ) const
	{
		return field_id_map.at( id );
	}
};
}
}

#endif //PQUANTUM_MODEL_SPECIFICATION_HPP
