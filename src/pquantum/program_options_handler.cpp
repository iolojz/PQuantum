//
// Created by jayz on 28.07.19.
//

#include "program_options_handler.hpp"
#include "calculation_controller.hpp"

#include "parsing/qft_json_parser.hpp"
#include "error/error.hpp"

#include <boost/log/attributes.hpp>

#include <iostream>

namespace po = boost::program_options;

namespace PQuantum {
program_options_handler::program_options_handler( int argc, const char **argv ) : program_options_description(
	"recognized options"
) {
	program_options_description.add_options()( "help", "produce this help message" )(
		"model-file",
		po::value<std::string>(
			&model_file
		), "model file"
	)(
		"calculate",
		po::value<std::vector<std::string>>(
			&calculation_command_names
		), "The things to calculate.\n"
			"Possible options:\n"
			"\tbeta_functions: Calculate the beta functions of the given model"
	)(
		"method",
		po::value<std::string>(
			&calculation_method_name
		), "The method to use for the calculation.\n"
	 		"Possible options:\n"
			"\tWetterich: Solve the Wetterich equation"
	);
	
	po::store( boost::program_options::parse_command_line( argc, argv, program_options_description ), program_options );
	boost::program_options::notify( program_options );
	
	if( program_options.count( "help" ) ) {
		print_help_message = true;
	} else {
		print_help_message = false;
	}
	
	logger.add_attribute( "object", boost::log::attributes::constant<std::string>( "program_options_parser" ) );
}

void program_options_handler::take_action( void ) {
	BOOST_LOG_NAMED_SCOPE( "take_action()" );
	
	if( print_help_message ) {
		BOOST_LOG_SEV( logger, logging::severity_level::user_info ) << program_options_description;
		return;
	}
	
	if( model_file.length() == 0 ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error ) << "No model file specified.";
		error::exit_upon_error();
	}
	
	BOOST_LOG_SEV( logger, logging::severity_level::user_info ) << "Attempting to parse model file:" << model_file;
	auto parser = parsing::qft_json_parser::parse( model_file );
	const auto &model = parser.model();
	BOOST_LOG_SEV( logger, logging::severity_level::user_info )
		<< "Successfully parsed model file: " << model_file << std::endl
		<< "JSON schema header: " << parser.json_header() << std::endl
		<< "Model: " << model.name();
	
	calculation_method method;
	std::vector<calculation_command> commands;
	for( const auto &command : calculation_command_names ) {
		if( command == "beta_functions" )
			commands.push_back( calculation_command::beta_functions );
		else {
			BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Unknown calculation command: " << command;
			error::exit_upon_error();
		}
	}
	if( calculation_method_name == "Wetterich" )
		method = calculation_method::wetterich;
	else {
		BOOST_LOG_SEV( logger, logging::severity_level::error )
			<< "Unknown calculation method: " << calculation_method_name;
		error::exit_upon_error();
	}
	
	calculation_controller controller = { commands, method };
	controller.calculate( model );
}
}
