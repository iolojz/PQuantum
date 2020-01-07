//
// Created by jayz on 29.07.19.
//

#include "error.hpp"
#include "logging/logging.hpp"

#include <cstdlib>

[[noreturn]] void PQuantum::error::exit_upon_error( void ) {
	logging::severity_logger logger;
	
	BOOST_LOG_NAMED_SCOPE("PQuantum::error::exit_upon_error()");
	BOOST_LOG_SEV(logger, logging::severity_level::user_info)
		<< "An error has occurred. Please see the error log for more details.";
	
	std::exit(EXIT_FAILURE);
}
