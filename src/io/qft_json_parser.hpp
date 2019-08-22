//
// Created by jayz on 25.07.19.
//

#ifndef PQUANTUM_QFT_JSON_PARSER_HPP
#define PQUANTUM_QFT_JSON_PARSER_HPP

#include <boost/property_tree/ptree_fwd.hpp>
#include <model/lagrangian.hpp>

#include "error/control.hpp"
#include "logging.hpp"

#include "qft_json_header.hpp"
#include "model/model_specification.hpp"

namespace PQuantum
{
namespace io
{
class qft_json_parser
{
	json_schema_header header;
	model::model_specification model_specification;
	
	qft_json_parser( const boost::property_tree::ptree &property_tree );
	
	static model::model_specification
	parse_model_specification( const io::json_schema_header &header, const boost::property_tree::ptree &property_tree );
	
	static mathutils::manifold_specification
	parse_manifold_specification( const boost::property_tree::ptree &property_tree );
	
	static mathutils::manifold_types::vector_space
	parse_vector_space_specification( const boost::property_tree::ptree &property_tree );
	
	static model::lagrangian parse_lagrangian( const boost::property_tree::ptree &property_tree );
	
	static model::classical_field_specification
	parse_field_specification( const boost::property_tree::ptree &property_tree );

public:
	static qft_json_parser parse( const std::string &path_to_file );
	
	const json_schema_header &json_header( void ) const
	{ return header; }
	
	const model::model_specification &model( void ) const
	{ return model_specification; }
};
}
}

#endif //PQUANTUM_QFT_JSON_PARSER_HPP
