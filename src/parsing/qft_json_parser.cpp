//
// Created by jayz on 01.08.19.
//

#include <iostream>

#include "qft_json_parser.hpp"
#include "parse_math.hpp"
#include "math_to_lagrangian.hpp"

#include "logging/logging.hpp"
#include "error/error.hpp"

#include "model/model_specification.hpp"
#include "model/lagrangian.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/iterator/transform_iterator.hpp>

namespace PQuantum::parsing {
static auto make_qft_parsing_context( boost::uuids::random_generator &uuid_generator,
	const std::map<std::string, support::uuid> &field_id_map,
	const std::map<std::string, support::uuid> &parameter_id_map,
	std::map<std::string, support::uuid> &index_id_map
) {
	auto field_id_lookup = [&field_id_map]( auto &&str ) -> std::optional<support::uuid> {
		auto field_id_it = field_id_map.find( str );
		if( field_id_it != field_id_map.end() ) {
			return {field_id_it->second};
		}
		
		return {};
	};
	auto parameter_id_lookup = [&parameter_id_map]( auto &&str ) -> std::optional<support::uuid> {
		auto parameter_id_it = parameter_id_map.find( str );
		if( parameter_id_it != parameter_id_map.end() ) {
			return {parameter_id_it->second};
		}
		
		return {};
	};
	auto index_id_lookup_and_generate = [&uuid_generator, &index_id_map](
		auto &&str
	) -> support::uuid {
		auto result = index_id_map.emplace( std::move( str ), uuid_generator );
		return {result.first->second};
	};
	
	return qft_parsing_context{field_id_lookup, parameter_id_lookup, index_id_lookup_and_generate};
}

qft_json_parser::qft_json_parser( const boost::property_tree::ptree &property_tree ) : header{
	property_tree.get<std::string>( "JSON schema.name" ), property_tree.get<int>( "JSON schema.version_major" ),
	property_tree.get<int>( "JSON schema.version_minor" )
}, model_specification{
	parse_model_specification( header, property_tree.get_child( "Quantum Field Theory" ) )
} {}

qft_json_parser qft_json_parser::parse( const std::string &path_to_file ) {
	BOOST_LOG_NAMED_SCOPE( "qft_json_parser::parse()" );
	logging::severity_logger logger;
	
	boost::property_tree::ptree property_tree;
	
	try {
		boost::property_tree::read_json( path_to_file, property_tree );
	} catch( const boost::property_tree::json_parser_error &exception ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Error parsing QFT JSON file: " << exception.what();
		error::exit_upon_error();
	}
	
	return qft_json_parser{std::move( property_tree )};
}

model::model_specification qft_json_parser::parse_model_specification( const json_schema_header &header,
	const boost::property_tree::ptree &property_tree
) {
	BOOST_LOG_NAMED_SCOPE( "qft_json_parser::parse_model_specification()" );
	logging::severity_logger logger;
	
	if( header.name != "QFT JSON" ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error )
			<< "The header name does not read \"QFT JSON\". Corrupt file?";
		error::exit_upon_error();
	}
	if( header.version_major != 0 || header.version_minor != 1 ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Incompatible header version "
				<< header.version_major
				<< "." << header.version_minor;
		error::exit_upon_error();
	}
	
	std::string model_name = property_tree.get<std::string>( "model name" );
	auto manifold_spec = parse_manifold_specification( property_tree.get_child( "topological manifold" ) );
	
	std::vector<model::classical_field_specification> field_specifications;
	std::transform(
		property_tree.get_child( "classical fields" ).begin(),
		property_tree.get_child( "classical fields" ).end(), std::back_inserter( field_specifications ),
		[&]( const auto &key_value_pair ) {
			const boost::property_tree::ptree &node = key_value_pair.second;
			return parse_field_specification( node );
		}
	);
	
	std::vector<model::classical_field_specification> conjugate_field_specifications;
	for( auto &&spec : field_specifications ) {
		if( spec.algebraic_field == mathutils::manifold_types::vector_space::algebraic_field::complex ) {
			conjugate_field_specifications.push_back(
				model::classical_field_specification{
					std::string{"\\conj{"} + spec.name + "}",
					spec.algebraic_field,
					spec.dimension
				}
			);
		} else if( spec.algebraic_field
			== mathutils::manifold_types::vector_space::algebraic_field::complex_grassmann ) {
			conjugate_field_specifications.emplace_back(
				model::classical_field_specification{
					std::string{"\\bar{"} + spec.name + "}",
					spec.algebraic_field,
					spec.dimension
				}
			);
		}
	}
	field_specifications.insert(
		field_specifications.end(),
		std::make_move_iterator( conjugate_field_specifications.begin() ),
		std::make_move_iterator( conjugate_field_specifications.end() )
	);
	
	
	std::map<support::uuid, model::classical_field_specification> field_id_map;
	std::map<std::string, support::uuid> field_name_map;
	for( auto &&spec : field_specifications ) {
		support::uuid id{uuid_generator};
		field_name_map.emplace( spec.name, id );
		field_id_map.emplace( id, std::move( spec ) );
	}
	
	std::vector<std::string> parameters;
	std::transform(
		property_tree.get_child( "parameters" ).begin(), property_tree.get_child( "parameters" ).end(),
		std::back_inserter( parameters ), [&]( const auto &node ) {
			return node.second.template get<std::string>( "" );
		}
	);
	
	std::map<support::uuid, std::string> parameter_id_map;
	std::map<std::string, support::uuid> parameter_name_map;
	for( auto &&p : parameters ) {
		parameter_id_map.emplace( uuid_generator, p );
		parameter_name_map.emplace( std::move( p ), uuid_generator );
	}
	
	auto lagrangian = parse_lagrangian(
		property_tree.get_child( "lagrangian" ), std::move( field_name_map ),
		std::move( parameter_name_map )
	);
	
	return {
		std::move( model_name ), std::move( manifold_spec ), std::move( field_id_map ), std::move( parameter_id_map ),
		std::move( lagrangian )
	};
}

model::classical_field_specification
qft_json_parser::parse_field_specification( const boost::property_tree::ptree &property_tree ) {
	std::string name = property_tree.get<std::string>( "name" );
	auto vector_space = parse_vector_space_specification( property_tree );
	
	return {std::move( name ), std::move( vector_space.field ), std::move( vector_space.dimension )};
}

mathutils::manifold_specification
qft_json_parser::parse_manifold_specification( const boost::property_tree::ptree &property_tree ) {
	BOOST_LOG_NAMED_SCOPE( "qft_json_parser::parse_manifold_specification()" );
	logging::severity_logger logger;
	
	std::string type = property_tree.get<std::string>( "type" );
	if( type != "vector space" ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Unrecognized manifold type: \"" << type << "\"";
		error::exit_upon_error();
	}
	
	return {parse_vector_space_specification( property_tree )};
}

mathutils::manifold_types::vector_space
qft_json_parser::parse_vector_space_specification( const boost::property_tree::ptree &property_tree ) {
	BOOST_LOG_NAMED_SCOPE( "qft_json_parser::parse_vector_space_specification()" );
	logging::severity_logger logger;
	
	mathutils::manifold_types::vector_space::algebraic_field field;
	std::string field_name = property_tree.get<std::string>( "algebraic field" );
	if( field_name == "real numbers" ) {
		field = mathutils::manifold_types::vector_space::algebraic_field::real;
	} else if( field_name == "complex numbers" ) {
		field = mathutils::manifold_types::vector_space::algebraic_field::complex;
	} else if( field_name == "complex grassmann numbers" ) {
		field = mathutils::manifold_types::vector_space::algebraic_field::complex_grassmann;
	} else {
		BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Unrecognized algebraic field: \"" << field_name
				<< "\"";
		error::exit_upon_error();
	}
	
	std::variant<int, mathutils::spacetime_dimension, mathutils::clifford_algebra_target_dimension> dimension;
	std::string dimension_string = property_tree.get<std::string>( "dimension" );
	if( dimension_string == "spacetime dimension" ) {
		dimension = mathutils::spacetime_dimension{};
	} else if( dimension_string == "clifford algebra target dimension" ) {
		dimension = mathutils::clifford_algebra_target_dimension{};
	} else {
		boost::optional<int> dim_boost = property_tree.get_optional<int>( "dimension" );
		if( dim_boost ) {
			dimension = *dim_boost;
		} else {
			BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Unrecognized dimension: \"" << dimension_string
					<< "\" encountered";
			error::exit_upon_error();
		}
	}
	
	mathutils::manifold_types::vector_space::vector_space_metric metric{
		mathutils::manifold_types::vector_space::vector_space_metric::unspecified
	};
	
	auto additional_data = property_tree.get_child_optional( "additional data" );
	if( additional_data ) {
		auto metric_name = additional_data->get_optional<std::string>( "metric" );
		if( metric_name ) {
			if( *metric_name == "euclidean" ) {
				metric = mathutils::manifold_types::vector_space::vector_space_metric::euclidean;
			} else if( *metric_name == "lorentzian" ) {
				metric = mathutils::manifold_types::vector_space::vector_space_metric::lorentzian;
			} else if( *metric_name == "complex nondegenerate" ) {
				metric = mathutils::manifold_types::vector_space::vector_space_metric::complex_nondegenerate;
			} else if( *metric_name == "unspecified" ) {
				metric = mathutils::manifold_types::vector_space::vector_space_metric::unspecified;
			} else {
				BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Unrecognized metric: \"" << *metric_name
						<< "\"";
				error::exit_upon_error();
			}
		}
	}
	
	return {std::move( field ), std::move( dimension ), std::move( metric )};
}

model::lagrangian_node qft_json_parser::parse_lagrangian( const boost::property_tree::ptree &property_tree,
	const std::map<std::string, support::uuid> &field_id_map,
	const std::map<std::string, support::uuid> &parameter_id_map
) {
	BOOST_LOG_NAMED_SCOPE( "model::parse_lagrangian()" );
	logging::severity_logger logger;
	
	support::tree::node_incarnation<mathutils::sum, model::lagrangian_tree_tag> lagrangian;
	std::transform(
		property_tree.begin(), property_tree.end(),
		std::back_inserter( lagrangian.children ), [&]( const auto &key_value_pair ) {
			const boost::property_tree::ptree &node = key_value_pair.second;
			std::string term = node.get<std::string>( "" );
			
			std::map<std::string, support::uuid> index_id_map;
			qft_parsing_context context = make_qft_parsing_context(
				uuid_generator, field_id_map, parameter_id_map,
				index_id_map
			);
			
			math_tree_node math_term = parse_math( term );
			return math_to_lagrangian( std::move( math_term ), std::move( context ) );
		}
	);
	return lagrangian;
}
}