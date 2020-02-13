//
// Created by jayz on 25.07.19.
//

#ifndef PQUANTUM_QFT_JSON_PARSER_HPP
#define PQUANTUM_QFT_JSON_PARSER_HPP

#include <boost/property_tree/ptree_fwd.hpp>

#include "qft_json_header.hpp"
#include "qft_parsing_context.hpp"

#include "model/model_specification.hpp"

#include <map>

namespace PQuantum::parsing {
class qft_json_parser {
	boost::uuids::random_generator uuid_generator;
	json_schema_header header;
	model::model_specification model_specification;
	
	qft_json_parser( const boost::property_tree::ptree &property_tree );
	
	model::model_specification
	parse_model_specification( const json_schema_header &header, const boost::property_tree::ptree &property_tree );
	
	mathutils::manifold_specification parse_manifold_specification( const boost::property_tree::ptree &property_tree );
	
	mathutils::manifold_types::vector_space
	parse_vector_space_specification( const boost::property_tree::ptree &property_tree );
	
	model::lagrangian_node parse_lagrangian( const boost::property_tree::ptree &property_tree,
											 const std::map<std::string, support::uuid> &field_id_map,
											 const std::map<std::string, support::uuid> &parameter_id_map );
	
	model::classical_field_specification parse_field_specification( const boost::property_tree::ptree &property_tree );
public:
	static qft_json_parser parse( const std::string &path_to_file );
	
	const json_schema_header &json_header( void ) const { return header; }
	
	const model::model_specification &model( void ) const
	{ return model_specification; }
};
}

#endif //PQUANTUM_QFT_JSON_PARSER_HPP
