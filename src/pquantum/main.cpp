#include "logging/logging.hpp"
#include "program_options_handler.hpp"

int main(int argc, const char **argv) {
	PQuantum::logging::setup_logging_facilities();
	PQuantum::program_options_handler po_handler{argc, argv};
	po_handler.take_action();
	
	return 0;
}