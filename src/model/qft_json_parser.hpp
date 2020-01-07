//
// Created by jayz on 25.07.19.
//

#ifndef PQUANTUM_QFT_JSON_PARSER_HPP
#define PQUANTUM_QFT_JSON_PARSER_HPP

#include <boost/property_tree/ptree_fwd.hpp>

#include "io/logging.hpp"
#include "io/qft_json_header.hpp"

#include "model/model_specification.hpp"

#include <map>

namespace PQuantum {
namespace io {
struct lagrangian_parsing_context {
	boost::uuids::random_generator &uuid_generator;
	const std::map<std::string, model::classical_field_id> &field_id_map;
	const std::map<std::string, mathutils::variable_id> &parameter_id_map;
	mutable std::map<std::string, boost::uuids::uuid> index_id_map;
	
	auto field_id_lookup(void) const {
		return [this](auto &&str) {
			BOOST_LOG_NAMED_SCOPE("field_id_lookup()");
			io::severity_logger logger;
			
			auto field_id_it = field_id_map.find(str);
			if(field_id_it != field_id_map.end())
				return field_id_it->second.id;
			
			BOOST_LOG_SEV(logger, severity_level::error) << "unknown field name '" << str << "'";
			error::exit_upon_error();
		};
	}
	
	auto parameter_id_lookup(void) const {
		return [this](auto &&str) {
			BOOST_LOG_NAMED_SCOPE("field_id_lookup()");
			io::severity_logger logger;
			
			auto parameter_id_it = parameter_id_map.find(str);
			if(parameter_id_it != parameter_id_map.end())
				return parameter_id_it->second.id;
			
			BOOST_LOG_SEV(logger, severity_level::error) << "unknown field name '" << str << "'";
			error::exit_upon_error();
		};
	}
	
	auto index_id_lookup_and_generate(void) const {
		return [this](auto &&str) {
			auto result = index_id_map.emplace(std::move(str), uuid_generator());
			return result.first->second;
		};
	}
};

class qft_json_parser {
	boost::uuids::random_generator uuid_generator;
	json_schema_header header;
	model::model_specification model_specification;
	
	qft_json_parser(const boost::property_tree::ptree &property_tree);
	
	model::model_specification
	parse_model_specification( const io::json_schema_header &header, const boost::property_tree::ptree &property_tree );
	
	mathutils::manifold_specification parse_manifold_specification( const boost::property_tree::ptree &property_tree );
	
	mathutils::manifold_types::vector_space
	parse_vector_space_specification( const boost::property_tree::ptree &property_tree );
	
	model::lagrangian parse_lagrangian(const boost::property_tree::ptree &property_tree,
									   const std::map<std::string, model::classical_field_id> &field_id_map,
									   std::map<std::string, mathutils::variable_id> parameter_id_map);
	
	model::classical_field_specification parse_field_specification( const boost::property_tree::ptree &property_tree );

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
