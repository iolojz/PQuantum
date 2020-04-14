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
	mutable boost::uuids::random_generator uuid_gen;
	std::map<support::uuid, classical_field_specification> field_id_map;
	std::map<support::uuid, std::string> parameter_id_map;
	input_lagrangian_tree input_lagrangian_;
	lagrangian_tree lagrangian_;
	
	logging::severity_logger logger;
	
	static lagrangian_tree to_lagrangian( boost::uuids::random_generator &, const input_lagrangian_tree & );
public:
	model_specification( std::string &&n, mathutils::manifold_specification &&m,
		boost::uuids::random_generator &&ug,
		std::map<support::uuid, classical_field_specification> &&fi,
		std::map<support::uuid, std::string> &&pi, input_lagrangian_tree &&il
	);
	
	const std::string &name( void ) const { return model_name; }
	const mathutils::manifold_specification &spacetime( void ) const { return spacetime_manifold; }
	support::uuid new_id( void ) const { return uuid_gen; }
	
	std::set<support::uuid> field_ids( void ) const {
		std::set<support::uuid> fields;
		for( const auto &id_entry : field_id_map ) {
			fields.emplace( id_entry.first );
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
	
	const input_lagrangian_tree &input_lagrangian( void ) const { return input_lagrangian_; }
	const lagrangian_tree &lagrangian( void ) const { return lagrangian_; }
};
}
}

#endif //PQUANTUM_MODEL_SPECIFICATION_HPP
