//
// Created by jayz on 01.08.19.
//

#include "qft_json_parser.hpp"

#include "error/control.hpp"
#include "logging.hpp"
#include "math_parser.hpp"

#include "model/model_specification.hpp"
#include "model/lagrangian.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace PQuantum
{
namespace io
{
qft_json_parser::qft_json_parser( const boost::property_tree::ptree &property_tree ) : header{
property_tree.get<std::string>( "JSON schema.name" ), property_tree.get<int>( "JSON schema.version_major" ),
property_tree.get<int>( "JSON schema.version_minor" ) }, model_specification{
parse_model_specification( header, property_tree.get_child( "Quantum Field Theory" )) }
{}

qft_json_parser qft_json_parser::parse( const std::string &path_to_file )
{
	BOOST_LOG_NAMED_SCOPE( "qft_json_parser::parse()" );
	io::severity_logger logger;
	
	boost::property_tree::ptree property_tree;
	
	try {
		boost::property_tree::read_json( path_to_file, property_tree );
	} catch( const boost::property_tree::json_parser_error &exception ) {
		BOOST_LOG_SEV( logger, io::severity_level::error ) << "Error parsing QFT JSON file: " << exception.what();
		control::exit_upon_error();
	}
	
	return qft_json_parser{ std::move( property_tree ) };
}

model::model_specification qft_json_parser::parse_model_specification( const io::json_schema_header &header,
																	   const boost::property_tree::ptree &property_tree )
{
	BOOST_LOG_NAMED_SCOPE( "qft_json_parser::parse_model_specification()" );
	io::severity_logger logger;
	
	if( header.name != "QFT JSON" ) {
		BOOST_LOG_SEV( logger, io::severity_level::error )
			<< "The header name does not read \"QFT JSON\". Corrupt file?";
		control::exit_upon_error();
	}
	if( header.version_major != 0 || header.version_minor != 1 ) {
		BOOST_LOG_SEV( logger, io::severity_level::error ) << "Incompatible header version " << header.version_major
														   << "." << header.version_minor;
		control::exit_upon_error();
	}
	
	std::string model_name = property_tree.get<std::string>( "model name" );
	auto manifold_spec = parse_manifold_specification( property_tree.get_child( "topological manifold" ));
	
	std::vector<model::classical_field_specification> field_specifications;
	std::transform( property_tree.get_child( "classical fields" ).begin(),
					property_tree.get_child( "classical fields" ).end(), std::back_inserter( field_specifications ),
					[&logger]( const auto &key_value_pair ) {
						const boost::property_tree::ptree &node = key_value_pair.second;
						return parse_field_specification( node );
					} );
	
	return { std::move( model_name ), std::move( manifold_spec ), std::move( field_specifications ) };
}

model::classical_field_specification
qft_json_parser::parse_field_specification( const boost::property_tree::ptree &property_tree )
{
	std::string name = property_tree.get<std::string>( "name" );
	auto vector_space = parse_vector_space_specification( property_tree );
	
	return { std::move( name ), std::move( vector_space.field ), std::move( vector_space.dimension ) };
}

mathutils::manifold_specification
qft_json_parser::parse_manifold_specification( const boost::property_tree::ptree &property_tree )
{
	BOOST_LOG_NAMED_SCOPE( "qft_json_parser::parse_manifold_specification()" );
	io::severity_logger logger;
	
	std::string type = property_tree.get<std::string>( "type" );
	if( type != "vector space" ) {
		BOOST_LOG_SEV( logger, io::severity_level::error ) << "Unrecognized manifold type: \"" << type << "\"";
		control::exit_upon_error();
	}
	
	return { parse_vector_space_specification( property_tree ) };
}

mathutils::manifold_types::vector_space
qft_json_parser::parse_vector_space_specification( const boost::property_tree::ptree &property_tree )
{
	BOOST_LOG_NAMED_SCOPE( "qft_json_parser::parse_vector_space_specification()" );
	io::severity_logger logger;
	
	mathutils::manifold_types::vector_space::algebraic_field field;
	std::string field_name = property_tree.get<std::string>( "algebraic field" );
	if( field_name == "real numbers" )
		field = mathutils::manifold_types::vector_space::algebraic_field::real;
	else if( field_name == "complex numbers" )
		field = mathutils::manifold_types::vector_space::algebraic_field::complex;
	else if( field_name == "complex grassmann numbers" )
		field = mathutils::manifold_types::vector_space::algebraic_field::complex_grassmann;
	else {
		BOOST_LOG_SEV( logger, io::severity_level::error ) << "Unrecognized algebraic field: \"" << field_name << "\"";
		control::exit_upon_error();
	}
	
	std::variant<int, mathutils::spacetime_dimension> dimension;
	std::string dimension_string = property_tree.get<std::string>( "dimension" );
	if( dimension_string == "spacetime dimension" )
		dimension = mathutils::spacetime_dimension{};
	else {
		boost::optional<int> dim_boost = property_tree.get_optional<int>( "dimension" );
		if( dim_boost )
			dimension = *dim_boost;
		else {
			BOOST_LOG_SEV( logger, io::severity_level::error ) << "Unrecognized dimension: \"" << dimension_string
															   << "\" encountered";
			control::exit_upon_error();
		}
	}
	
	mathutils::manifold_types::vector_space::vector_space_metric metric;
	auto additional_data = property_tree.get_child_optional( "additional data" );
	if( additional_data ) {
		auto metric_name = additional_data->get_optional<std::string>( "metric" );
		if( metric_name ) {
			if( *metric_name == "euclidean" )
				metric = mathutils::manifold_types::vector_space::vector_space_metric::euclidean;
			else if( *metric_name == "lorentzian" )
				metric = mathutils::manifold_types::vector_space::vector_space_metric::lorentzian;
			else if( *metric_name == "complex nondegenerate" )
				metric = mathutils::manifold_types::vector_space::vector_space_metric::complex_nondegenerate;
			else if( *metric_name == "unspecified" )
				metric = mathutils::manifold_types::vector_space::vector_space_metric::unspecified;
			else {
				BOOST_LOG_SEV( logger, io::severity_level::error ) << "Unrecognized metric: \"" << *metric_name << "\"";
				control::exit_upon_error();
			}
		}
	}
	
	return { std::move( field ), std::move( dimension ), std::move( metric ) };
}

model::lagrangian qft_json_parser::parse_lagrangian( const boost::property_tree::ptree &property_tree )
{
	BOOST_LOG_NAMED_SCOPE( "model::parse_lagrangian()" );
	io::severity_logger logger;
	
	model::lagrangian terms;
	std::for_each( property_tree.begin(), property_tree.end(), [&logger, &terms]( const auto &key_value_pair ) {
		const boost::property_tree::ptree &node = key_value_pair.second;
		
		std::string monomial_string = node.get<std::string>( "monomial" );
		std::string coefficient_string = node.get<std::string>( "coefficient" );
		auto constant_factor = node.get_optional<std::string>( "constant factor" );
		
		auto symbols = io::parse_symbols<model::lagrangian_symbol>( monomial_string );
		
		model::lagrangian term{ io::parse_polynomial_expression( std::move( coefficient_string )),
								std::make_move_iterator( symbols.begin()), std::make_move_iterator( symbols.end()) };
		
		if( constant_factor )
			term *= model::lagrangian{ io::parse_polynomial_expression( *constant_factor ) };
		
		terms += std::move( term );
	} );
	
	return terms;
}
}
}