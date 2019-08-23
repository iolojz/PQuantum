//
// Created by jayz on 28.07.19.
//

#include "program_options_handler.hpp"
#include "qft_json_parser.hpp"
#include "error/error.hpp"

#include <boost/log/attributes.hpp>

#include <iostream>

namespace po = boost::program_options;

namespace PQuantum
{
namespace io
{
program_options_handler::program_options_handler( int argc, const char **argv ) : program_options_description(
"recognized options" )
{
	program_options_description.add_options()( "help", "produce this help message" )( "model-file",
																					  po::value<std::string>(
																					  &model_file ), "model file" );
	
	po::store( boost::program_options::parse_command_line( argc, argv, program_options_description ), program_options );
	boost::program_options::notify( program_options );
	
	if( program_options.count( "help" ))
		print_help_message = true;
	else
		print_help_message = false;
	
	logger.add_attribute( "object", boost::log::attributes::constant<std::string>( "program_options_parser" ));
}

void program_options_handler::take_action( void )
{
	BOOST_LOG_NAMED_SCOPE( "take_action()" );
	
	if( print_help_message ) {
		BOOST_LOG_SEV( logger, io::severity_level::user_info ) << program_options_description;
		return;
	}
	
	if( model_file.length() == 0 ) {
		BOOST_LOG_SEV( logger, io::severity_level::error ) << "No model file specified.";
		error::exit_upon_error();
	}
	
	qft_json_parser::parse( model_file );
}
}
}
