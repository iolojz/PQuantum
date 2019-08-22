//
// Created by jayz on 29.07.19.
//

#include "control.hpp"
#include "io/logging.hpp"

#include <cstdlib>

[[noreturn]] void PQuantum::control::exit_upon_error( void )
{
	io::severity_logger logger;
	
	BOOST_LOG_NAMED_SCOPE( "PQuantum::control::exit_upon_error()" );
	BOOST_LOG_SEV( logger, io::severity_level::user_info )
		<< "An error has occurred. Please see the error log for more details.";
	
	std::exit( EXIT_FAILURE );
}
