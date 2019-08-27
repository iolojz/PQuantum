//
// Created by jayz on 28.07.19.
//

#ifndef PQUANTUM_PROGRAM_OPTIONS_HANDLER_HPP
#define PQUANTUM_PROGRAM_OPTIONS_HANDLER_HPP

#include "logging.hpp"

#include <boost/program_options.hpp>

#include <string>

namespace PQuantum
{
namespace io
{
class program_options_handler
{
	boost::program_options::options_description program_options_description;
	boost::program_options::variables_map program_options;
	
	bool print_help_message;
	std::string model_file;
	
	severity_logger logger;
public:
	program_options_handler( int argc, const char **argv );
	
	void take_action( void );
};
}
}

#endif //PQUANTUM_PROGRAM_OPTIONS_HANDLER_HPP