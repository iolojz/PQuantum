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

namespace PQuantum {
namespace io {
struct json_schema_header;
}

namespace model {
class model_specification {
	std::string model_name;
	mathutils::manifold_specification spacetime_manifold;
	std::map<support::uuid, classical_field_specification> field_id_map;
	std::map<support::uuid, std::string> parameter_id_map;
	lagrangian_tree lagrangian;
public:
	model_specification( std::string &&n, mathutils::manifold_specification &&m,
		std::map<support::uuid, classical_field_specification> &&fi,
		std::map<support::uuid, std::string> &&pi, lagrangian_tree &&l
	);
	
	const std::string &name( void ) const { return model_name; }
	
	const mathutils::manifold_specification &spacetime( void ) const { return spacetime_manifold; }
	
	std::vector<support::uuid> field_ids( void ) const {
		std::vector<support::uuid> fields;
		for( const auto &id_entry : field_id_map ) {
			fields.push_back( id_entry.first );
		}
		return fields;
	}
	
	const classical_field_specification &field_specification_for_id( const support::uuid &id ) const {
		return field_id_map.at( id );
	}
	
	std::vector<support::uuid> parameter_ids( void ) const {
		std::vector<support::uuid> parameters;
		for( const auto &id_entry : parameter_id_map ) {
			parameters.push_back( id_entry.first );
		}
		return parameters;
	}
	
	const std::string &parameter_name_for_id( const support::uuid &id ) const {
		return parameter_id_map.at( id );
	}
	
	const lagrangian_tree &model_lagrangian( void ) const { return lagrangian; }
};
}
}

#endif //PQUANTUM_MODEL_SPECIFICATION_HPP
