#include "io/logging.hpp"
#include "io/program_options_handler.hpp"

int main( int argc, const char **argv )
{
	PQuantum::io::setup_logging_facilities();
	PQuantum::io::program_options_handler po_handler{ argc, argv };
	po_handler.take_action();
	
	return 0;
}